// C++ Standard Library
#include <iostream>

// Local Headers
#include <web.h>

std::string url_encode(const std::string &value)
{
    std::ostringstream escaped;
    escaped.fill('0');
    escaped << std::hex;

    for (char c : value)
    {
        if (isalnum(static_cast<unsigned char>(c)) || c == '-' || c == '_' || c == '.' || c == '~')
        {
            escaped << c;
        }
        else
        {
            escaped << '%' << std::setw(2) << int(static_cast<unsigned char>(c));
        }
    }

    return escaped.str();
}

std::string WebMatch::getDisplay() const
{
    return "Search the web for: " + input;
}

RunResult WebMatch::run()
{
    pid_t pid = fork();
    if (pid == 0) // Child process
    {
        // Detach from parent process
        setsid();
        // Execute the command using the shell
        std::string query = searchUrl + url_encode(input);
        std::string command = "xdg-open \"" + query + "\"";
        if (std::system(command.c_str()))
        {
            std::cerr << "Failed to execute command: " << command << std::endl;
        }
        _exit(127); // Exit child if execl fails
    }
    else if (pid < 0) // Fork failed
    {
        return CLOSE;
    }
    // Parent continues without waiting
    return CLOSE;
}

double WebMatch::getRelevance(const std::string &input) const
{
    if (input.empty())
    {
        return 0;
    }
    return relevance;
}

std::vector<Match *> Web::getMatches(const std::string &input) const
{
    if (input.empty())
    {
        return {};
    }
    return {
        new WebMatch(
            input,
            pluginSettings.value("relevance", 0.5),
            pluginSettings.value("searchUrl", "https://www.google.com/search?q="))};
}