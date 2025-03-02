#ifndef __plugin_h__
#define __plugin_h__
#include <string>
#include <vector>
#include "match.h"
class Plugin
{
public:
    virtual float matchChance(std::string input) { return 1; };
    virtual std::vector<Match *> getMatches(std::string input) { return {}; };
    virtual std::string getPluginName() { return ""; };
    virtual ~Plugin() {};
};
#endif