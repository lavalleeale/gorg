#ifndef __finder_h__
#define __finder_h__
#include <vector>
#include "plugins/plugin.h"
#include "plugins/match.h"
class Finder
{
public:
    Finder();
    ~Finder();
    RunResult RunMatch();
    void find(const std::string &query);
    const std::vector<Match *> &getMatches() const { return matches; }

private:
    std::vector<Plugin *>
        plugins;
    std::vector<Match *> matches;
};
#endif