#pragma once

#include <string>
#include <vector>

struct Mod {
    std::string name;
    std::string version;
    std::string author;
    std::string gameVersion;
    std::string directory;
};

class ModLoader {
private:
    std::vector<Mod> mods;

public:
    void loadMods();
    const std::vector<Mod>& getMods() const;
};