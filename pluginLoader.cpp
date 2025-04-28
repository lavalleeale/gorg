#include <stdexcept>
#include <filesystem>
#include <iostream>
#include <pluginLoader.h>

#ifdef _WIN32
#define GET_SYMBOL GetProcAddress
#else
#define GET_SYMBOL dlsym
#endif

std::vector<Plugin *> loadPluginDirectory(const std::string &path)
{
    std::vector<Plugin *> plugins;
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
                Plugin *plugin = loadPlugin(entry.path().string());
                plugins.push_back(plugin);
            }
            catch (const std::exception &e)
            {
                // Handle loading error (e.g., log it)
                std::cerr << "Failed to load plugin: " << e.what() << std::endl;
            }
        }
    }

    return plugins;
}

Plugin *loadPlugin(const std::string &path)
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

#ifdef _WIN32
    auto create = (CreateFunc)GET_SYMBOL((HMODULE)handle, "create");
#else
    auto create = (CreateFunc)GET_SYMBOL(handle, "create");
#endif

    if (!create)
        throw std::runtime_error("Missing create symbol");

    return create();
}
