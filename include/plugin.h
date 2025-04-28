#ifndef __plugin_h__
#define __plugin_h__

// C++ Standard Library
#include <string>
#include <vector>

// Local Headers
#include <match.h>

#include <nlohmann/json.hpp>

class Plugin
{
public:
    virtual std::vector<Match *> getMatches(const std::string &) const { return {}; };
    virtual std::string getName() const { return "Plugin"; };
    virtual void setSettings(const nlohmann::json &settings) { pluginSettings = settings; };
    virtual ~Plugin() {};

protected:
    // Plugin settings
    nlohmann::json pluginSettings;
};
#endif