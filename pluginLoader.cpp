#include <stdexcept>
#include <filesystem>
#include <iostream>
#include <pluginLoader.h>

#ifdef _WIN32
#define GET_SYMBOL GetProcAddress
#else
#define GET_SYMBOL dlsym
#endif

namespace
{
using ApiVersionFunc = int (*)();

void closePluginLibrary(decltype(LoadedPlugin::handle) handle)
{
#ifndef _WIN32
    if (handle)
    {
        dlclose(handle);
    }
#else
    if (handle)
    {
        FreeLibrary(handle);
    }
#endif
}
}

LoadedPlugin::LoadedPlugin(LoadedPlugin &&other) noexcept
    : handle(other.handle), plugin(std::move(other.plugin))
{
    other.handle = nullptr;
}

LoadedPlugin &LoadedPlugin::operator=(LoadedPlugin &&other) noexcept
{
    if (this != &other)
    {
        plugin.reset();
        closePluginLibrary(handle);
        handle = other.handle;
        plugin = std::move(other.plugin);
        other.handle = nullptr;
    }
    return *this;
}

LoadedPlugin::~LoadedPlugin()
{
    plugin.reset();
    closePluginLibrary(handle);
}

std::vector<LoadedPlugin> loadPluginDirectory(const std::string &path)
{
    std::vector<LoadedPlugin> plugins;
    namespace fs = std::filesystem;
    // check if the user has plugins
    if (!fs::is_directory(path))
    {
        return plugins;
    }
    for (const auto &entry : fs::directory_iterator(path))
    {
        if (entry.is_regular_file() && (entry.path().extension() == ".dll" || entry.path().extension() == ".so"))
        {
            try
            {
                plugins.push_back(loadPlugin(entry.path().string()));
            }
            catch (const std::exception &e)
            {
                // Handle loading error (e.g., log it)
                std::cerr << "Failed to load plugin " << entry.path() << ": " << e.what() << std::endl;
            }
        }
    }

    return plugins;
}

LoadedPlugin loadPlugin(const std::string &path)
{
#ifdef _WIN32
    auto handle = LoadLibraryA(path.c_str());
    if (!handle)
        throw std::runtime_error("LoadLibraryA failed");
#else
    auto handle = dlopen(path.c_str(), RTLD_LAZY);
    if (!handle)
        throw std::runtime_error(dlerror());
#endif

    using CreateFunc = Plugin *(*)();
    using VersionFunc = ApiVersionFunc;

#ifdef _WIN32
    auto version = (VersionFunc)GET_SYMBOL((HMODULE)handle, "gorg_plugin_api_version");
    auto create = (CreateFunc)GET_SYMBOL((HMODULE)handle, "create");
#else
    auto version = (VersionFunc)GET_SYMBOL(handle, "gorg_plugin_api_version");
    auto create = (CreateFunc)GET_SYMBOL(handle, "create");
#endif

    if (!version)
    {
        closePluginLibrary(handle);
        throw std::runtime_error("Missing gorg_plugin_api_version symbol");
    }
    if (version() != GORG_PLUGIN_API_VERSION)
    {
        closePluginLibrary(handle);
        throw std::runtime_error("Incompatible plugin API version");
    }
    if (!create)
    {
        closePluginLibrary(handle);
        throw std::runtime_error("Missing create symbol");
    }

    Plugin *plugin = create();
    if (!plugin)
    {
        closePluginLibrary(handle);
        throw std::runtime_error("create returned null");
    }

    return LoadedPlugin(handle, std::unique_ptr<Plugin>(plugin));
}
