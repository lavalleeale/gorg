#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <filesystem>
#include <map>
#include <algorithm>
#include "match.h"
#include "drun.h"
#include <stringMatch.h>
#include <unistd.h>

#define PATH_SEPARATOR ":"

float Drun::matchChance(std::string input)
{
    if (input.empty())
    {
        return 0;
    }
    return 0.5;
}

std::vector<Match *> Drun::getMatches(std::string input)
{
    std::vector<Match *> binaries;
    char *path_env = std::getenv("PATH");
    if (path_env == nullptr)
    {
        return binaries;
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
                if (fuzzyMatchScore(input, entry.path().filename()) > 0)
                {
                    if (((std::filesystem::status(entry).permissions() & std::filesystem::perms::owner_exec) != std::filesystem::perms::none) && std::filesystem::is_regular_file(entry))
                    {
                        binaries.push_back(new DrunMatch(entry.path()));
                    }
                }
            }
            catch (const std::filesystem::filesystem_error &e)
            {
                continue;
            }
        }
    }
    return binaries;
}

std::string DrunMatch::getDisplay()
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

double DrunMatch::getRelevance(std::string input)
{
    return 0.5 * fuzzyMatchScore(input, path.filename().string());
}
