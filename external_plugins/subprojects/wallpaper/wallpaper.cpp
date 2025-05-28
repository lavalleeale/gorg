#include <iostream>
#include "wallpaper.h"
#include <stringMatch.h>
#include <unistd.h>
#include <sys/types.h>

std::string WallpaperMatch::getDisplay() const
{
    return "Change wallpaper to: " + value.stem().string();
}

std::string WallpaperMatch::getIcon() const
{
    return value.string();
}

RunResult WallpaperMatch::run()
{
    if (pluginSettings.find("commands") == pluginSettings.end())
    {
        std::cerr << "No commands found in plugin settings." << std::endl;
        return CLOSE;
    }
    pid_t pid = fork();
    if (pid == 0)
    {
        setsid();
        for (const auto &command : pluginSettings["commands"])
        {
            if (!command.is_string())
            {
                std::cerr << "Command is not a string: " << command.dump() << std::endl;
                continue;
            }
            std::cout << "Executing command: " << command.dump() << std::endl;
            std::string cmd = command.get<std::string>();
            if (cmd.empty())
            {
                std::cerr << "Command is empty." << std::endl;
                continue;
            }
            // Replace placeholder with the actual wallpaper path
            auto pos = cmd.find("$wallpaper");
            if (pos != std::string::npos)
            {
                cmd.replace(pos, 10, value.string());
            }
            if (system(cmd.c_str()) != 0)
            {
                std::cerr << "Failed to execute command: " << cmd << std::endl;
            }
        }
        // Exit the child process
        _exit(0);
    }
    return CLOSE;
}

double WallpaperMatch::getRelevance(const std::string &input) const
{
    return fuzzyMatchScore(input, value);
}

std::vector<Match *> WallpaperPlugin::getMatches(const std::string &input) const
{
    // get options array from pluginSettings
    std::vector<Match *> matches;
    if (pluginSettings.find("wallpaperDir") == pluginSettings.end())
    {
        std::cerr << "No wallpaperDir found in plugin settings." << std::endl;
    }
    else
    {
        const std::string wallpaperDir = pluginSettings.value("wallpaperDir", "");
        if (wallpaperDir.empty())
        {
            std::cerr << "Wallpaper directory is empty." << std::endl;
            return matches;
        }
        // Recursively search for files in the wallpaper directory
        for (const auto &entry : std::filesystem::recursive_directory_iterator(wallpaperDir))
        {
            if (entry.is_regular_file())
            {
                std::string value = entry.path();
                // Check if the input matches the file name
                if (hasAllChars(input, value))
                {
                    matches.push_back(new WallpaperMatch(value, pluginSettings));
                }
            }
        }
    }
    return matches;
}

extern "C" Plugin *create()
{
    return new WallpaperPlugin();
}