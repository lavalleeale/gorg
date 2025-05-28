#ifndef __wallpaper_h__
#define __wallpaper_h__
#include <plugin.h>
#include <nlohmann/json.hpp>
#include <settings.h>

class WallpaperMatch : public Match
{
public:
    WallpaperMatch(const std::filesystem::path &value, const nlohmann::json &pluginSettings) : value(value), pluginSettings(pluginSettings) {};
    std::string getDisplay() const override;
    std::string getIcon() const override;
    RunResult run() override;
    double getRelevance(const std::string &input) const override;
    ~WallpaperMatch() override {};

private:
    std::filesystem::path value;
    nlohmann::json pluginSettings;
};

class WallpaperPlugin : public Plugin
{
public:
    std::vector<Match *> getMatches(const std::string &input) const override;
    std::string getName() const override { return "wallpaper"; };

private:
};
extern "C" Plugin *create();
#endif