#ifndef __plugin_h__
#define __plugin_h__
#include <string>
#include <vector>
#include "match.h"
class Plugin
{
public:
    virtual std::vector<Match *> getMatches(const std::string &) const { return {}; };
    virtual ~Plugin() {};
};
#endif