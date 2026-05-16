#ifndef __pluginLoader_h__
#define __pluginLoader_h__
#include <string>
#include <memory>
#include <vector>
#include <plugin.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

struct LoadedPlugin
{
#ifndef _WIN32
    void *handle = nullptr;
#else
    HMODULE handle = nullptr;
#endif
    std::unique_ptr<Plugin> plugin;

    LoadedPlugin() = default;
    LoadedPlugin(decltype(handle) handle, std::unique_ptr<Plugin> plugin) : handle(handle), plugin(std::move(plugin)) {}
    LoadedPlugin(const LoadedPlugin &) = delete;
    LoadedPlugin &operator=(const LoadedPlugin &) = delete;
    LoadedPlugin(LoadedPlugin &&other) noexcept;
    LoadedPlugin &operator=(LoadedPlugin &&other) noexcept;
    ~LoadedPlugin();
};

LoadedPlugin loadPlugin(const std::string &path);
std::vector<LoadedPlugin> loadPluginDirectory(const std::string &path);
#endif
