#ifndef __finder_h__
#define __finder_h__

// C++ Standard Library
#include <vector>

// Local Headers
#include <match.h>
#include <plugin.h>

class Finder
{
public:
    Finder();
    Finder(const std::vector<std::string> &modes);
    ~Finder();
    RunResult RunMatch();
    void find(const std::string &query);
    const std::vector<Match *> &getMatches() const { return matches; }

private:
    void loadPluginSettings();
    std::vector<Plugin *>
        plugins;
    std::vector<Match *> matches;
};
#endif