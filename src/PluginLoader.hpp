#pragma once

#include <string>
#include <vector>
#include <memory>

#include "ModSDK/GWEnemyAPI.hpp"

#ifdef _WIN32
    #include <windows.h>
    using LibraryHandle = HMODULE;
#else
    using LibraryHandle = void*;
#endif

struct RegisteredEnemy {
    std::string id;
    GWEnemyDefinition definition;
    LibraryHandle pluginHandle;
};

struct LoadedPlugin {
    LibraryHandle handle;
    std::string path;
};

class PluginLoader {
private:
    std::vector<LoadedPlugin> plugins;
    std::vector<RegisteredEnemy> enemies;

    static int GW_CALL registerEnemy(
        void* context,
        const GWEnemyDefinition* definition
    );

public:
    PluginLoader() = default;
    ~PluginLoader();

    bool load(const std::string& path);

    const std::vector<RegisteredEnemy>& getEnemies() const;

    void unloadAll();
};