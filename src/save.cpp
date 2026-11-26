/*
**  File: Goblin-Warfare-PC-Edition/src/save.cpp
**
**  Creator: Aleksander Sapieha (sourceguy68@gmail.com)
**
**  Contributors:
**  Aleksander Sapieha (sourceguy68@gmail.com)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdexcept>
#include <cstdio>
#include <cstdlib>
#include <stdexcept>
#include <iostream>

extern "C" {
    #include "SDD/sdd.h"
    #include "SDD/sdd_parser.h"
}
#include "Player.hpp"
#include "save.hpp"


Value serializePlayer(const Player& player) {
    Value result = value_object();

    object_set(&result.data.object, "name",
        value_string(player.getName().c_str()));

    object_set(&result.data.object, "health",
        value_integer(player.getHealth()));

    object_set(&result.data.object, "hunger",
        value_integer(player.getHunger()));

    object_set(&result.data.object, "level",
        value_integer(player.getLevel()));

    object_set(&result.data.object, "xp",
        value_integer(player.getXp()));

    object_set(&result.data.object, "xpTilNextLevel",
        value_integer(player.getXpTilNextLevel()));

    object_set(&result.data.object, "score",
        value_integer(player.getScore()));

    object_set(&result.data.object, "gold",
        value_integer(player.getGold()));

    object_set(&result.data.object, "damage",
        value_integer(player.getDamage()));

    char armorBuffer[64];
    snprintf(armorBuffer, sizeof(armorBuffer), "%.9f",
        static_cast<double>(player.getArmor()));

    object_set(&result.data.object, "armor",
        value_decimal(armorBuffer));

    object_set(&result.data.object, "hasScriptures",
        value_boolean(player.getScripturesStatus()));

    // Serialize inventory
    Value inventory = value_array();

    for (const std::string& item : player.getInventory()) {
        array_append(
            &inventory.data.array,
            value_string(item.c_str())
        );
    }

    object_set(&result.data.object, "inventory", inventory);

    object_set(&result.data.object, "maxHealth",
        value_integer(player.getMaxHealth()));

    return result;
}

// Find an object field
static const Value* getField(
    const Value& object,
    const char* name
) {
    if (object.type != VALUE_OBJECT)
        return nullptr;

    for (size_t i = 0; i < object.data.object.count; i++) {
        const ObjectEntry& entry =
            object.data.object.entries[i];

        if (std::string(entry.key) == name)
            return entry.value;
    }

    return nullptr;
}

// Convert SDD Value into Player
Player deserializePlayer(const Value& data) {
    if (data.type != VALUE_OBJECT)
        throw std::runtime_error("Expected an SDD object");

    const Value* name = getField(data, "name");
    const Value* health = getField(data, "health");
    const Value* hunger = getField(data, "hunger");
    const Value* level = getField(data, "level");
    const Value* xp = getField(data, "xp");
    const Value* xpTilNextLevel = getField(data, "xpTilNextLevel");
    const Value* score = getField(data, "score");
    const Value* gold = getField(data, "gold");
    const Value* damage = getField(data, "damage");
    const Value* armor = getField(data, "armor");
    const Value* hasScriptures = getField(data, "hasScriptures");
    const Value* inventoryValue = getField(data, "inventory");
    const Value* maxHealth = getField(data, "maxHealth");

    // Check that every field exists
    if (!name || !health || !hunger || !level || !xp ||
        !xpTilNextLevel || !score || !gold || !damage ||
        !armor || !hasScriptures || !inventoryValue || !maxHealth) {
        throw std::runtime_error("Missing Player field in SDD");
    }

    // Check field types
    if (name->type != VALUE_STRING ||
        health->type != VALUE_INTEGER ||
        hunger->type != VALUE_INTEGER ||
        level->type != VALUE_INTEGER ||
        xp->type != VALUE_INTEGER ||
        xpTilNextLevel->type != VALUE_INTEGER ||
        score->type != VALUE_INTEGER ||
        gold->type != VALUE_INTEGER ||
        damage->type != VALUE_INTEGER ||
        armor->type != VALUE_DECIMAL ||
        hasScriptures->type != VALUE_BOOLEAN ||
        inventoryValue->type != VALUE_ARRAY ||
        maxHealth->type != VALUE_INTEGER) {
        throw std::runtime_error("Invalid Player field type");
    }

    // Deserialize inventory
    std::list<std::string> inventory;

    for (size_t i = 0; i < inventoryValue->data.array.count; i++) {
        const Value& item =
            inventoryValue->data.array.items[i];

        if (item.type != VALUE_STRING)
            throw std::runtime_error("Invalid inventory item");

        inventory.push_back(item.data.string);
    }

    // Convert armor from decimal string
    char* end = nullptr;

    float armorNumber = strtof(
        armor->data.decimal,
        &end
    );

    if (end == armor->data.decimal || *end != '\0')
        throw std::runtime_error("Invalid armor value");

    // Reconstruct Player
    return Player(
        name->data.string,
        static_cast<int>(health->data.integer),
        static_cast<int>(hunger->data.integer),
        static_cast<int>(level->data.integer),
        static_cast<int>(xp->data.integer),
        static_cast<int>(xpTilNextLevel->data.integer),
        static_cast<int>(score->data.integer),
        static_cast<int>(gold->data.integer),
        static_cast<int>(damage->data.integer),
        armorNumber,
        hasScriptures->data.boolean,
        inventory,
        static_cast<int>(maxHealth->data.integer)
    );
}


void save(Player player){
    Value data = serializePlayer(player);

    SDDError error;
    char* output = nullptr;

    if (sdd_dump(&data, "player.sdd", &error)) {
        std::cout << output;
        free(output);
    } else {
        std::cerr << error.message << '\n';
    }

    value_destroy(&data);
}


Player load() {
    SDDDocument data{};
    SDDError error{};

    if (!sdd_load("player.sdd", &data, &error)) {
        std::cerr << error.message << '\n';
        throw std::runtime_error("Failed to load player");
    }

    try {
        Player player = deserializePlayer(data.values);

        sdd_document_destroy(&data);

        return player;
    }
    catch (...) {
        sdd_document_destroy(&data);
        throw;
    }
}
