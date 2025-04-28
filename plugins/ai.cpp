// C++ Standard Library
#include <iostream>
#include <future>

// External Libraries
#include <curl/curl.h>
#include <nlohmann/json.hpp>

// Local Headers
#include <ai.h>
#include <settings.h>

using json = nlohmann::json;

// This callback gets called for each chunk of data received.
size_t AIMatch::WriteCallback(char *ptr, size_t size, size_t nmemb, void *userdata)
{
    AIMatch *self = static_cast<AIMatch *>(userdata);
    size_t totalSize = size * nmemb;
    std::string data(ptr, totalSize);

    // Process each line of data (assuming chunks are newline-delimited)
    size_t pos = 0;
    while ((pos = data.find("\n")) != std::string::npos)
    {
        std::string line = data.substr(0, pos);
        data.erase(0, pos + 1);

        // Sometimes the stream sends a 'data: ' prefix
        if (line.find("data: ") == 0)
        {
            line = line.substr(6);
        }
        if (line.empty())
        {
            continue;
        }

        // Check for the stream termination signal (if provided by API)
        if (line == "[DONE]")
        {
            continue;
        }

        try
        {
            auto j = json::parse(line);
            // Extract the new content delta
            if (!j.contains("choices") || j["choices"].empty() || !j["choices"][0].contains("delta") || !j["choices"][0]["delta"].contains("content"))
            {
                continue;
            }
            std::string content = j["choices"][0]["delta"]["content"];
            if (self->responseText == "Thinking...")
            {
                self->responseText = "";
            }
            self->responseText += content;
            self->signal_update_label.emit();
        }
        catch (json::parse_error &e)
        {
            // Handle incomplete JSON chunks or parsing errors.
            std::cerr << "JSON parse error: " << e.what() << std::endl;
        }
    }

    return totalSize;
}

RunResult AIMatch::run()
{
    // Set initial response text from settings
    responseText = "Thinking...";
    signal_update_label.emit();

    CURL *curl = curl_easy_init();
    if (!curl)
    {
        std::cerr << "Failed to initialize curl." << std::endl;
        return CONTINUE;
    }

    // Construct your JSON request payload
    json requestPayload = {
        {"model", model},
        {"stream", true},
        {"messages",
         {
             {{"role", "user"}, {"content", input}},
         }},
    };

    std::string requestData = requestPayload.dump();
    std::string authHeader = "Authorization: Bearer " + apiKey;

    struct curl_slist *headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, authHeader.c_str());

    // Use std::async to manage the thread's lifetime explicitly:
    auto backgroundTask = std::async(std::launch::async, [this, curl, headers, requestData]()
                                     {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, requestData.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK)
        {
            std::cerr << "curl_easy_perform() failed: "
                      << curl_easy_strerror(res) << std::endl;
            responseText += "\nError: Failed to get response.";
            signal_update_label.emit();
        }
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl); });

    return TAKEOVER;
}

AIMatch::AIMatch(const std::string &input, double relevance, int max_width, const std::string &url, const std::string &apiKey, const std::string &model)
    : input(input), url(url), apiKey(apiKey), model(model), relevance(relevance), responseText("")
{
    // Make sure lines wrap properly
    label.set_line_wrap(true);
    label.set_line_wrap_mode(Pango::WrapMode::WRAP_WORD_CHAR);
    label.set_max_width_chars(max_width);
    label.set_size_request(50, -1);

    // Connect the signal to update the label text
    signal_update_label.connect([this]()
                                { label.set_text(this->responseText); });
}

std::string AIMatch::getDisplay() const
{
    return "Ask AI: " + input;
}

double AIMatch::getRelevance(const std::string &input) const
{
    if (input.empty())
    {
        return 0;
    }
    return relevance;
}

std::vector<Match *> AI::getMatches(const std::string &input) const
{
    if (input.empty())
    {
        return {};
    }
    return {
        new AIMatch(
            input,
            pluginSettings.value("relevance", 0.5),
            pluginSettings.value("maxWidth", 50),
            pluginSettings.value("apiUrl", "http://localhost:11434/v1/chat/completions"),
            pluginSettings.value("apiKey", ""),
            pluginSettings.value("model", "llama2:7b"))};
}
