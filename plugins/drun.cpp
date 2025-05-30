// C++ Standard Library
#include <vector>

// Local Headers
#include <drun.h>
#include <match.h>
#include <stringMatch.h>

#define PATH_SEPARATOR ":"

std::vector<Match *> Drun::getMatches(const std::string &input) const
{
    std::vector<Match *> binaries;
    for (const auto &item : cache)
    {
        if (hasAllChars(input, item->getDisplay()))
        {
            binaries.push_back(item);
        }
    }
    return binaries;
}

std::string DrunMatch::getDisplay() const
{
    return path.filename().string();
}

RunResult DrunMatch::run()
{
    pid_t pid = fork();
    if (pid == 0) // Child process
    {
        // Detach from parent process
        setsid();
        // Execute the command using the shell
        execl(path.c_str(), path.c_str(), nullptr);
        _exit(127); // Exit child if execl fails
    }
    else if (pid < 0) // Fork failed
    {
        return CLOSE;
    }
    // Parent continues without waiting
    return CLOSE;
}

double DrunMatch::getRelevance(const std::string &input) const
{
    return relevance * fuzzyMatchScore(input, path.filename().string());
}

void Drun::setSettings(const nlohmann::json &settings)
{
    pluginSettings = settings;
    char *path_env = std::getenv("PATH");
    if (path_env == nullptr)
    {
        return;
    }
    std::string path_str(path_env);
    size_t start = 0;
    size_t end = path_str.find(PATH_SEPARATOR);
    while (end != std::string::npos)
    {
        std::string dir = path_str.substr(start, end - start);
        start = end + 1;
        end = path_str.find(PATH_SEPARATOR, start);
        if (!std::filesystem::is_directory(dir))
        {
            continue;
        }
        for (const auto &entry : std::filesystem::directory_iterator(dir))
        {
            try
            {
                if (((std::filesystem::status(entry).permissions() & std::filesystem::perms::owner_exec) != std::filesystem::perms::none) && std::filesystem::is_regular_file(entry))
                {
                    cache.push_back(new DrunMatch(entry.path(), pluginSettings.value("relevance", 0.5)));
                }
            }
            catch (const std::filesystem::filesystem_error &e)
            {
                continue;
            }
        }
    }
}