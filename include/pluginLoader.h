#ifndef __pluginLoader_h__
#define __pluginLoader_h__
#include <string>
#include <vector>
#include <plugin.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

Plugin *loadPlugin(const std::string &path);
std::vector<Plugin *> loadPluginDirectory(const std::string &path);
#endif