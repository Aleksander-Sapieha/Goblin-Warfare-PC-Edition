#include "PluginLoader.hpp"

#include <iostream>
#include <cstring>

#ifdef _WIN32
    #define GW_LOAD(path) LoadLibraryA(path)
    #define GW_SYMBOL(handle, name) GetProcAddress(handle, name)
    #define GW_CLOSE(handle) FreeLibrary(handle)
#else
    #include <dlfcn.h>

    #define GW_LOAD(path) dlopen(path, RTLD_NOW | RTLD_LOCAL)
    #define GW_SYMBOL(handle, name) dlsym(handle, name)
    #define GW_CLOSE(handle) dlclose(handle)
#endif

using ModInitFunction = int (GW_CALL *)(const GWHostAPI*);

PluginLoader::~PluginLoader() {
    unloadAll();
}

int GW_CALL PluginLoader::registerItem(
    void* context,
    const GWItemDefinition* definition
) {
    auto* loader = static_cast<PluginLoader*>(context);

    if (!loader || !definition)
        return 0;

    if (definition->struct_size != sizeof(GWItemDefinition))
        return 0;

    if (!definition->id ||
        !definition->name ||
        !definition->on_use) {
        return 0;
    }

    for (const auto& existing : loader->items) {
        if (existing.id == definition->id) {
            std::cerr << "[PLUGIN] Duplicate item ID: "
                      << definition->id << std::endl;
            return 0;
        }
    }

    RegisteredItem item{};
    item.id = definition->id;
    item.definition = *definition;
    item.pluginHandle = nullptr;

    loader->items.push_back(item);

    std::cout << "[PLUGIN] Registered item: "
              << item.id << std::endl;

    return 1;
}

int GW_CALL PluginLoader::registerEnemy(
    void* context,
    const GWEnemyDefinition* definition
) {
    auto* loader = static_cast<PluginLoader*>(context);

    if (!loader || !definition) {
        return 0;
    }

    if (definition->struct_size != sizeof(GWEnemyDefinition)) {
        std::cerr << "[PLUGIN] Incompatible enemy structure size!"
                  << std::endl;
        return 0;
    }

    if (!definition->id ||
        !definition->create ||
        !definition->destroy ||
        !definition->get_name ||
        !definition->get_health ||
        !definition->get_damage ||
        !definition->get_gold_reward ||
        !definition->take_damage ||
        !definition->attack) {
        std::cerr << "[PLUGIN] Invalid enemy definition!"
                  << std::endl;
        return 0;
    }

    for (const auto& existing : loader->enemies) {
        if (existing.id == definition->id) {
            std::cerr << "[PLUGIN] Duplicate enemy ID: "
                      << definition->id << std::endl;
            return 0;
        }
    }

    RegisteredEnemy enemy{};
    enemy.id = definition->id;
    enemy.definition = *definition;
    enemy.pluginHandle = nullptr;

    loader->enemies.push_back(enemy);

    std::cout << "[PLUGIN] Registered enemy: "
              << enemy.id << std::endl;

    return 1;
}
bool PluginLoader::load(const std::string& path) {
    LibraryHandle handle = GW_LOAD(path.c_str());

    if (!handle) {
        std::cout << "[PLUGIN] Failed to load: "
                  << path << std::endl;

#ifndef _WIN32
        const char* error = dlerror();

        if (error) {
            std::cout << "[PLUGIN] " << error << std::endl;
        }
#endif

        return false;
    }

    auto init = reinterpret_cast<ModInitFunction>(
        GW_SYMBOL(handle, "gw_mod_init")
    );

    if (!init) {
        std::cout << "[PLUGIN] Missing gw_mod_init: "
                  << path << std::endl;

        GW_CLOSE(handle);
        return false;
    }

    GWHostAPI host{};
    host.api_version = GW_API_VERSION;
    host.context = this;
    host.register_enemy = registerEnemy;

    size_t previousCount = enemies.size();

    if (!init(&host)) {
        std::cout << "[PLUGIN] Initialization failed: "
                  << path << std::endl;

        enemies.resize(previousCount);
        GW_CLOSE(handle);
        return false;
    }

    for (size_t i = previousCount; i < enemies.size(); i++) {
        enemies[i].pluginHandle = handle;
    }

    plugins.push_back({handle, path});

    std::cout << "[PLUGIN] Loaded: "
              << path << std::endl;

    return true;
}

const std::vector<RegisteredEnemy>&
PluginLoader::getEnemies() const {
    return enemies;
}

void PluginLoader::unloadAll() {
    // Remove all registered callbacks before unloading libraries.
    enemies.clear();

    for (auto& plugin : plugins) {
        if (plugin.handle) {
            GW_CLOSE(plugin.handle);
        }
    }

    plugins.clear();
}