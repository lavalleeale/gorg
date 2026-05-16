#ifndef __finder_h__
#define __finder_h__

// C++ Standard Library
#include <vector>
#include <map>
#include <memory>
#include <string>

// Local Headers
#include <match.h>
#include <plugin.h>
#include <pluginLoader.h>

class Finder
{
public:
    Finder(const std::vector<std::string> &modes = {},
           const std::map<std::string, std::map<std::string, std::string>> &pluginOptions = {});
    ~Finder();
    RunResult RunMatch();
    void find(const std::string &query);
    const std::vector<Match *> &getMatches() const { return matches; }

private:
    void loadPluginSettings(const std::map<std::string, std::map<std::string, std::string>> &pluginOptions = {});
    std::vector<std::unique_ptr<Plugin>> plugins;
    std::vector<LoadedPlugin> loadedPlugins;
    std::vector<Match *> matches;
};
#endif
