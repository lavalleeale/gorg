#include <iostream>
#include "obsidian.h"
#include <stringMatch.h>

std::string urlEncode(const std::string &text)
{
    std::ostringstream encoded;
    encoded.fill('0');
    encoded << std::hex;

    for (char c : text)
    {
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~')
        {
            encoded << c;
        }
        else
        {
            encoded << '%' << std::setw(2) << int((unsigned char)c);
        }
    }
    return encoded.str();
}

std::string ObsidianMatch::getDisplay() const
{
    return "Open note: " + value.filename().string();
}

RunResult ObsidianMatch::run()
{
    pid_t pid = fork();
    if (pid == 0)
    {
        setsid();
        std::vector<std::string> args;
        args.push_back("xdg-open");
        args.push_back("obsidian://open?path=" + urlEncode(value.string()));
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

double ObsidianMatch::getRelevance(const std::string &input) const
{
    return fuzzyMatchScore(input, value.string());
}

void ObsidianPlugin::setSettings(const nlohmann::json &settings)
{
    pluginSettings = settings;
    // Load notes from Obsidian vault
    std::string vaultPath = pluginSettings.value("vaultPath", "");
    if (vaultPath.empty())
    {
        std::cerr << "Vault path not set in settings." << std::endl;
        return;
    }
    for (const auto &entry : std::filesystem::recursive_directory_iterator(vaultPath))
    {
        if (entry.is_regular_file() && entry.path().extension() == ".md")
        {
            notes.push_back(entry.path());
        }
    }
}

std::vector<Match *> ObsidianPlugin::getMatches(const std::string &input) const
{
    std::vector<Match *> matches;
    for (const auto &note : notes)
    {
        if (hasAllChars(input, note.string()))
        {
            matches.push_back(new ObsidianMatch(note));
        }
    }
    return matches;
}

extern "C" Plugin *create()
{
    return new ObsidianPlugin();
}