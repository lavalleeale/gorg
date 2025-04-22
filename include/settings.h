#ifndef __settings_h__
#define __settings_h__

// Third Party Libraries
#include <nlohmann/json.hpp>

int getWindowWidth();
int getWindowHeight();
unsigned int getMaxResults();
std::string getCustomCss();
nlohmann::json getPluginSettings(const std::string &pluginName);
std::string getLastQuery();
void saveLastQuery(const std::string &query);

#endif