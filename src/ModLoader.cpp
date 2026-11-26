#include <filesystem>
#include <iostream>
#include <cstring>

#include "ModLoader.hpp"
#include "PluginLoader.hpp"
extern "C"{
    #include "SDD/sdd.h"
}

namespace fs = std::filesystem;

static Value* getValue(Object* object, const char* key) {
    for (size_t i = 0; i < object->count; i++) {
        if (strcmp(object->entries[i].key, key) == 0) {
            return object->entries[i].value;
        }
    }

    return nullptr;
}

static std::string getString(Object* object, const char* key) {
    Value* value = getValue(object, key);

    if (!value || value->type != VALUE_STRING) {
        return "";
    }

    return value->data.string;
}



void ModLoader::loadMods() {
    pluginLoader.unloadAll();
    mods.clear();

    if (!fs::exists("mods")) {
        fs::create_directories("mods");
        return;
    }

    for (const auto& entry : fs::directory_iterator("mods")) {
        if (!entry.is_directory()) {
            continue;
        }

        fs::path metadataPath = entry.path() / "mod.sdd";

        if (!fs::exists(metadataPath)) {
            std::cout << "[MOD] Missing mod.info: "
                      << entry.path().filename().string()
                      << std::endl;
            continue;
        }

        SDDDocument document{};
        SDDError error{};

        if (!sdd_load(metadataPath.string().c_str(), &document, &error)) {
            std::cout << "[MOD] Failed to load "
                      << entry.path().filename().string()
                      << ": " << error.message << std::endl;
            continue;
        }

        if (document.values.type != VALUE_OBJECT) {
            std::cout << "[MOD] Invalid metadata structure."
                      << std::endl;

            sdd_document_destroy(&document);
            continue;
        }

        Object* object = &document.values.data.object;

        Mod mod;
        mod.name = getString(object, "name");
        mod.version = getString(object, "version");
        mod.author = getString(object, "author");
        mod.gameVersion = getString(object, "game_version");
        mod.directory = entry.path().string();

        if (mod.name.empty() ||
            mod.version.empty() ||
            mod.gameVersion.empty()) {

            std::cout << "[MOD] Missing required metadata: "
                      << entry.path().filename().string()
                      << std::endl;

            sdd_document_destroy(&document);
            continue;
        }

        if (mod.gameVersion != "5") {
            std::cout << "[MOD] Incompatible game version: "
                      << mod.name << std::endl;

            sdd_document_destroy(&document);
            continue;
        }

        #ifdef _WIN32
            fs::path pluginPath = entry.path() / "enemies.dll";
        #else
            fs::path pluginPath = entry.path() / "enemies.so";
        #endif

        if (!fs::exists(pluginPath)) {
            std::cout << "[MOD] Missing native plugin: "
                      << mod.name << std::endl;

            sdd_document_destroy(&document);
            continue;
        }

        if (!pluginLoader.load(pluginPath.string())) {
            std::cout << "[MOD] Failed to load plugin: "
                      << mod.name << std::endl;

            sdd_document_destroy(&document);
            continue;
        }

        mods.push_back(mod);

        std::cout << "[MOD] Loaded "
                  << mod.name << " v" << mod.version
                  << " by " << mod.author << std::endl;

        sdd_document_destroy(&document);
    }

    std::cout << "[MOD] Total loaded: "
              << mods.size() << std::endl;
}

const std::vector<Mod>& ModLoader::getMods() const {
    return mods;
}

PluginLoader& ModLoader::getPluginLoader() {
    return pluginLoader;
}