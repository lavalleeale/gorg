#ifndef __finder_h__
#define __finder_h__
#include <vector>
#include "plugin.h"
#include "drun.h"
#include "equation.h"
#include "match.h"
class Finder
{
public:
    Finder();
    ~Finder();
    bool RunMatch();
    void find(const std::string &query);
    const std::vector<Match *> &getMatches() const { return matches; }

private:
    std::vector<Plugin *>
        plugins;
    std::vector<Match *> matches;
};
#endif