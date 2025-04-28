#ifndef __plugin_h__
#define __plugin_h__

// C++ Standard Library
#include <string>
#include <vector>

// Local Headers
#include <match.h>

class Plugin
{
public:
    virtual std::vector<Match *> getMatches(const std::string &) const { return {}; };
    virtual std::string getName() const { return "Plugin"; };
    virtual ~Plugin() {};
};
#endif