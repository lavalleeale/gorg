#include <iostream>
#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp> // For JSON parsing; install from https://github.com/nlohmann/json

using json = nlohmann::json;

// This callback gets called for each chunk of data received.
size_t WriteCallback(char *ptr, size_t size, size_t nmemb, void *userdata)
{
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
            std::cout << "Stream completed." << std::endl;
            continue;
        }

        try
        {
            auto j = json::parse(line);
            std::cout << "Received message: " << j["choices"][0]["delta"]["content"] << std::endl;
            std::cout << j["choices"].size() << std::endl;
        }
        catch (json::parse_error &e)
        {
            // Handle incomplete JSON chunks or parsing errors.
            std::cerr << "JSON parse error: " << e.what() << std::endl;
        }
    }

    return totalSize;
}

// int main()
// {
//     CURL *curl = curl_easy_init();
//     if (!curl)
//     {
//         std::cerr << "Failed to initialize curl." << std::endl;
//         return 1;
//     }

//     // OpenAI API URL for chat completions
//     const std::string url = "http://localhost:11434/v1/chat/completions";

//     // Construct your JSON request payload
//     json requestPayload = {
//         {"model", "llama3.2:1b"},
//         {"stream", true},
//         {"messages",
//          {
//              {{"role", "user"}, {"content", "Hello, how are you?"}},
//          }},
//     };

//     std::string requestData = requestPayload.dump();

//     struct curl_slist *headers = nullptr;
//     // Set your OpenAI API key here
//     std::string apiKey = "Bearer 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";
//     headers = curl_slist_append(headers, ("Authorization: " + apiKey).c_str());
//     headers = curl_slist_append(headers, "Content-Type: application/json");

//     curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
//     curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
//     curl_easy_setopt(curl, CURLOPT_POSTFIELDS, requestData.c_str());
//     curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);

//     // For streaming, you may want to disable buffering:
//     // curl_easy_setopt(curl, CURLOPT_BUFFERSIZE, 1024L);

//     CURLcode res = curl_easy_perform(curl);
//     if (res != CURLE_OK)
//     {
//         std::cerr << "curl_easy_perform() failed: "
//                   << curl_easy_strerror(res) << std::endl;
//     }

//     curl_slist_free_all(headers);
//     curl_easy_cleanup(curl);
//     return 0;
// }
