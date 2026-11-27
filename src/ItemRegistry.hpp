#pragma once

#include <string>
#include <vector>

struct GameItem {
    std::string id;
    std::string name;
    int price;
    std::string mod;
    std::string effect;
    int amount;
};

class ItemRegistry {
private:
    std::vector<GameItem> items;

public:
    void registerItem(const GameItem& item);
    void registerVanillaItems();

    void loadModItems(
        const std::string& directory,
        const std::string& modName
    );

    const std::vector<GameItem>& getItems() const;

    const GameItem* getItem(const std::string& name) const;
    const GameItem* getItemByID(const std::string& id) const;
};