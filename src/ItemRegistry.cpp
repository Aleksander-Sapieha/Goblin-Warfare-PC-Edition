
#include <iostream>
#include <cstring>
#include <limits>

#include "ItemRegistry.hpp"

extern "C" {
    #include "SDD/sdd.h"
}

static Value* getValue(Object* object, const char* key) {
    for (size_t i = 0; i < object->count; i++) {
        if (strcmp(object->entries[i].key, key) == 0) {
            return object->entries[i].value;
        }
    }

    return nullptr;
}

void ItemRegistry::registerItem(const GameItem& item) {
    if (getItem(item.name) != nullptr) {
        std::cout << "[ITEM] Duplicate item ignored: "
                  << item.name << std::endl;
        return;
    }

    items.push_back(item);
}

void ItemRegistry::registerVanillaItems() {
    registerItem({"1", "Health Potion", 100, "Vanilla", "heal", 100});
    registerItem({"2", "Steak", 10, "Vanilla", "food", 10});
    registerItem({"3", "Apple", 20, "Vanilla", "food", 10});
    registerItem({"4", "Holy Scriptures", 5000, "Vanilla", "none", 0});
    registerItem({"5", "LVLUP Potion", 100, "Vanilla", "xp", 100});
    registerItem({"6", "Leather", 10, "Vanilla", "none", 0});
}

void ItemRegistry::loadModItems(
    const std::string& directory,
    const std::string& modName
) {
    std::string filepath = directory + "/items.sdd";

    SDDDocument document{};
    SDDError error{};

    if (!sdd_load(filepath.c_str(), &document, &error)) {
        std::cout << "[ITEM] Could not load items from "
                  << modName << ": " << error.message << std::endl;
        return;
    }

    if (document.values.type != VALUE_OBJECT) {
        std::cout << "[ITEM] Invalid items file: "
                  << modName << std::endl;
        sdd_document_destroy(&document);
        return;
    }

    Value* itemArray = getValue(
        &document.values.data.object,
        "items"
    );

    if (!itemArray || itemArray->type != VALUE_ARRAY) {
        std::cout << "[ITEM] Missing items array: "
                  << modName << std::endl;
        sdd_document_destroy(&document);
        return;
    }

    for (size_t i = 0; i < itemArray->data.array.count; i++) {
        Value* itemValue = &itemArray->data.array.items[i];

        if (itemValue->type != VALUE_OBJECT) {
            continue;
        }

        Object* object = &itemValue->data.object;

        Value* name = getValue(object, "name");
        Value* price = getValue(object, "price");
        Value* effect = getValue(object, "effect");
        Value* amount = getValue(object, "amount");

        if (!name || !price ||
            name->type != VALUE_STRING ||
            price->type != VALUE_INTEGER) {
            std::cout << "[ITEM] Invalid item definition in "
                      << modName << std::endl;
            continue;
        }

        if (!name->data.string ||
            name->data.string[0] == '\0' ||
            price->data.integer < 0 ||
            price->data.integer > std::numeric_limits<int>::max()) {
            std::cout << "[ITEM] Invalid item properties in "
                      << modName << std::endl;
            continue;
        }

        std::string effectName = "none";
        int effectAmount = 0;

        if (effect) {
            if (effect->type != VALUE_STRING ||
                !effect->data.string) {
                continue;
            }

            effectName = effect->data.string;
        }

        if (amount) {
            if (amount->type != VALUE_INTEGER ||
                amount->data.integer < 0 ||
                amount->data.integer > std::numeric_limits<int>::max()) {
                continue;
            }

            effectAmount = static_cast<int>(amount->data.integer);
        }

        if (effectName != "none" &&
            effectName != "heal" &&
            effectName != "food" &&
            effectName != "xp" &&
            effectName != "gold") {
            std::cout << "[ITEM] Unknown effect: "
                      << effectName << std::endl;
            continue;
        }

        GameItem item{};
        item.name = name->data.string;
        item.price = static_cast<int>(price->data.integer);
        item.mod = modName;
        item.effect = effectName;
        item.amount = effectAmount;

        registerItem(item);

        std::cout << "[ITEM] Registered "
                  << item.name << " from " << modName
                  << std::endl;
    }

    sdd_document_destroy(&document);
}

const std::vector<GameItem>& ItemRegistry::getItems() const {
    return items;
}

const GameItem* ItemRegistry::getItem(
    const std::string& name
) const {
    for (const GameItem& item : items) {
        if (item.name == name) {
            return &item;
        }
    }

    return nullptr;
}
