// C++ Standard Library
#include <fstream>
#include <iostream>

// Project Headers
#include <run.h>
#include <stringMatch.h>

std::vector<Match *> Run::getMatches(const std::string &input) const
{
    std::vector<Match *> matches;
    for (const auto &item : cache)
    {
        if (hasAllChars(input, item->getDisplay()))
        {
            matches.push_back(item);
        }
    }
    return matches;
}

std::string RunMatch::getDisplay() const
{
    return name;
}

RunResult RunMatch::run()
{
    pid_t pid = fork();
    if (pid == 0)
    {
        setsid();
        std::istringstream iss(exec);
        std::vector<std::string> args;
        std::string token;
        while (iss >> token)
        {
            // Skip arguments that start with %
            if (!token.empty() && token[0] != '%')
            {
                args.push_back(token);
            }
        }
        // Build argv vector; safe to use c_str() as strings persist in args vector.
        std::vector<char *> argv;
        for (auto &arg : args)
        {
            argv.push_back(const_cast<char *>(arg.c_str()));
        }
        argv.push_back(nullptr);
        execvp(argv[0], argv.data());
        std::cerr << "Failed to execute " << argv[0] << std::endl;
        _exit(127);
    }
    return CLOSE;
}

double RunMatch::getRelevance(const std::string &input) const
{
    return relevance * fuzzyMatchScore(input, name);
}

void Run::setSettings(const nlohmann::json &settings)
{
    pluginSettings = settings;
    char *data_env = std::getenv("XDG_DATA_DIRS");
    if (data_env == nullptr)
    {
        return;
    }
    std::string data_str(data_env);
    size_t start = 0;
    size_t end = data_str.find(':');
    while (start != std::string::npos)
    {
        std::string dir = data_str.substr(start, end - start) + "/applications";
        if (end == std::string::npos)
        {
            start = end;
        }
        else
        {
            start = end + 1;
        }
        end = data_str.find(':', start);
        if (!std::filesystem::is_directory(dir))
        {
            continue;
        }
        for (const auto &entry : std::filesystem::directory_iterator(dir))
        {
            if (entry.path().extension() == ".desktop")
            {
                std::ifstream file(entry.path());
                if (!file.is_open())
                {
                    continue;
                }
                std::string line;
                std::string name_line;
                std::string icon_line;
                std::string exec_line;
                while (std::getline(file, line))
                {
                    if (line.find("Name=") == 0)
                    {
                        name_line = line.substr(5);
                    }
                    if (line.find("Exec=") == 0)
                    {
                        exec_line = line.substr(5);
                    }
                    if (line.find("Icon=") == 0)
                    {
                        icon_line = line.substr(5);
                    }
                    if (!name_line.empty() && !exec_line.empty() && !icon_line.empty())
                    {
                        break;
                    }
                }
                file.close();
                if (!name_line.empty())
                {
                    RunMatch *match = new RunMatch(exec_line, name_line, icon_line, pluginSettings.value("relevance", 0.75));
                    cache.push_back(match);
                }
            }
        }
    }
}