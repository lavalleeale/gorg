#ifndef __settings_h__
#define __settings_h__
#include <nlohmann/json.hpp>
#include <string>
#include <mutex>

int getWindowWidth();
int getWindowHeight();
int getMaxResults();
std::string getCustomCss();
nlohmann::json getPluginSettings(const std::string &pluginName);
#endif