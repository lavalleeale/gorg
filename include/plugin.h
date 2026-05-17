#ifndef __plugin_h__
#define __plugin_h__

// C++ Standard Library
#include <string>
#include <vector>

// Local Headers
#include <match.h>

#include <nlohmann/json.hpp>

#define GORG_PLUGIN_API_VERSION 1

class Plugin
{
public:
    // Returned matches are borrowed by the caller and must remain valid until
    // the plugin is updated or destroyed.
    virtual std::vector<Match *> getMatches(const std::string &) const { return {}; };
    virtual std::string getName() const { return "Plugin"; };
    virtual void setSettings(const nlohmann::json &settings) { pluginSettings = settings; };
    virtual ~Plugin() {};

protected:
    // Plugin settings
    nlohmann::json pluginSettings;
};
#endif
