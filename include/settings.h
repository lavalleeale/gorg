#ifndef __settings_h__
#define __settings_h__
#include <nlohmann/json.hpp>
#include <string>

class Settings
{
public:
    static Settings &getInstance();
    void loadFromDir(const std::string &directory);
    void load();

    int getWindowWidth() const { return windowWidth; }
    int getWindowHeight() const { return windowHeight; }
    int getMaxResults() const { return maxResults; }
    const std::string &getCustomCss() const { return customCss; }
    const nlohmann::json &getPluginSettings(const std::string &pluginName) const;

private:
    Settings() = default;
    int windowWidth = 600;
    int windowHeight = 200;
    int maxResults = 25;
    std::string customCss;
    nlohmann::json pluginSettings;
};
#endif