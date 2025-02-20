#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <filesystem>
#include <map>
#include <algorithm>
#include "match.h"
#include "drun.h"
#include "levenshtein.h"

#define PATH_SEPARATOR ":"

float Drun::matchChance(std::string input)
{
    if (input.empty())
    {
        return 0;
    }
    return 1;
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
                if (((std::filesystem::status(entry).permissions() & std::filesystem::perms::owner_exec) != std::filesystem::perms::none) && std::filesystem::is_regular_file(entry))
                {
                    binaries.push_back(new DrunMatch(entry.path()));
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

float DrunMatch::getRelevance(std::string input)
{
    return 1.0 / levenshteinDist(input, path.filename().string());
}
