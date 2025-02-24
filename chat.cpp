#include <iostream>
#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include "chat.h"
#include <thread>

using json = nlohmann::json;

// This callback gets called for each chunk of data received.
size_t WriteCallback(char *ptr, size_t size, size_t nmemb, void *userdata)
{
    ChatMatch *chatMatch = static_cast<ChatMatch *>(userdata);
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
            chatMatch->updateResponse(j["choices"][0]["delta"]["content"]);
        }
        catch (json::parse_error &e)
        {
            // Handle incomplete JSON chunks or parsing errors.
            std::cerr << "JSON parse error: " << e.what() << std::endl;
        }
    }

    return totalSize;
}

RunResult ChatMatch::run()
{
    CURL *curl = curl_easy_init();
    if (!curl)
    {
        std::cerr << "Failed to initialize curl." << std::endl;
        return CONTINUE;
    }

    // OpenAI API URL for chat completions
    const std::string url = "http://localhost:11434/v1/chat/completions";

    // Construct your JSON request payload
    json requestPayload = {
        {"model", "llama3.2:1b"},
        {"stream", true},
        {"messages",
         {
             {{"role", "user"}, {"content", input}},
         }},
    };

    std::string requestData = requestPayload.dump();

    struct curl_slist *headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");

    // Replace the synchronous call with a background thread:
    std::thread backgroundThread([curl, headers, requestData, url, chatMatchPointer = this]()
                                 {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, requestData.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, chatMatchPointer);

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK)
        {
            std::cerr << "curl_easy_perform() failed: "
                      << curl_easy_strerror(res) << std::endl;
        }
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl); });
    backgroundThread.detach();

    return TAKEOVER;
}

ChatMatch::ChatMatch(std::string input)
{
    this->input = input;
    // Make sure lines wrap properly
    label.set_line_wrap(true);
    label.set_line_wrap_mode(Pango::WrapMode::WRAP_WORD_CHAR);
    label.set_max_width_chars(40);
    label.set_size_request(50, -1);
}

std::string ChatMatch::getDisplay()
{
    return "Ask ChatGPT: " + input;
}

double ChatMatch::getRelevance(std::string input)
{
    return 0.75;
}

void ChatMatch::updateResponse(std::string response)
{
    std::string newText = label.get_text() + response;
    label.set_text(newText);
}
