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
    if (self->cancelled)
    {
        return 0;
    }
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
            {
                std::lock_guard<std::mutex> lock(self->responseMutex);
                if (self->responseText == "Thinking...")
                {
                    self->responseText = "";
                }
                self->responseText += content;
            }
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

int AIMatch::ProgressCallback(void *clientp, curl_off_t, curl_off_t, curl_off_t, curl_off_t)
{
    AIMatch *self = static_cast<AIMatch *>(clientp);
    return self->cancelled ? 1 : 0;
}

RunResult AIMatch::run()
{
    // Set initial response text from settings
    {
        std::lock_guard<std::mutex> lock(responseMutex);
        responseText = "Thinking...";
    }
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

    if (backgroundTask.valid())
    {
        cancelled = true;
        backgroundTask.wait();
    }
    cancelled = false;

    backgroundTask = std::async(std::launch::async, [this, curl, headers, requestData]()
                                     {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, requestData.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
        curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, ProgressCallback);
        curl_easy_setopt(curl, CURLOPT_XFERINFODATA, this);
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK && !cancelled)
        {
            std::cerr << "curl_easy_perform() failed: "
                      << curl_easy_strerror(res) << std::endl;
            {
                std::lock_guard<std::mutex> lock(responseMutex);
                responseText += "\nError: Failed to get response.";
            }
            signal_update_label.emit();
        }
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl); });

    return TAKEOVER;
}

AIMatch::AIMatch(const std::string &input, double relevance, int max_width, const std::string &url, const std::string &apiKey, const std::string &model)
    : input(input), responseText(""), relevance(relevance), url(url), apiKey(apiKey), model(model)
{
    // Make sure lines wrap properly
    label.set_line_wrap(true);
    label.set_line_wrap_mode(Pango::WrapMode::WRAP_WORD_CHAR);
    label.set_max_width_chars(max_width);
    label.set_size_request(50, -1);

    // Connect the signal to update the label text
    signal_update_label.connect([this]()
                                {
                                    std::lock_guard<std::mutex> lock(responseMutex);
                                    label.set_text(this->responseText);
                                });
}

AIMatch::~AIMatch()
{
    cancelled = true;
    if (backgroundTask.valid())
    {
        backgroundTask.wait();
    }
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

void AIMatch::updateInput(const std::string &input)
{
    this->input = input;
}

std::vector<Match *> AI::getMatches(const std::string &input) const
{
    if (input.empty())
    {
        return {};
    }
    if (!currentMatch)
    {
        return {};
    }
    currentMatch->updateInput(input);
    return {currentMatch.get()};
}

void AI::setSettings(const nlohmann::json &settings)
{
    pluginSettings = settings;
    currentMatch = std::make_unique<AIMatch>(
        settings.value("input", ""),
        settings.value("relevance", 0.5),
        settings.value("maxWidth", 50),
        settings.value("apiUrl", "https://api.openai.com/v1/chat/completions"),
        settings.value("apiKey", ""),
        settings.value("model", "gpt-3.5-turbo"));
}
