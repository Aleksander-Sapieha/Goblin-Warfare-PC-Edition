
#include "GWEnemyAPI.hpp"
#include <new>

class Dragon {
public:
    int health = 200;
};

static void* GW_CALL createDragon() {
    return new Dragon();
}

static void GW_CALL destroyDragon(void* enemy) {
    delete static_cast<Dragon*>(enemy);
}

static const char* GW_CALL getDragonName(void*) {
    return "Dragon";
}

static int32_t GW_CALL getDragonHealth(void* enemy) {
    if (!enemy) return 0;
    return static_cast<Dragon*>(enemy)->health;
}

static int32_t GW_CALL getDragonDamage(void*) {
    return 10;
}

static int32_t GW_CALL getDragonGold(void*) {
    return 500;
}

static void GW_CALL damageDragon(void* enemy, int32_t damage) {
    if (!enemy || damage < 0) return;

    auto* dragon = static_cast<Dragon*>(enemy);
    dragon->health -= damage;

    if (dragon->health < 0) {
        dragon->health = 0;
    }
}

static void GW_CALL attackDragon(
    void* enemy,
    const GWPlayerAPI* player
) {
    if (!enemy || !player || !player->take_damage) {
        return;
    }

    player->take_damage(player->context, getDragonDamage(enemy));
}

static const GWEnemyDefinition dragonDefinition = {
    sizeof(GWEnemyDefinition),
    "example.dragon",
    createDragon,
    destroyDragon,
    getDragonName,
    getDragonHealth,
    getDragonDamage,
    getDragonGold,
    damageDragon,
    attackDragon
};

extern "C" GW_API int GW_CALL gw_mod_init(
    const GWHostAPI* host
) {
    if (!host ||
        host->api_version != GW_API_VERSION ||
        !host->register_enemy) {
        return 0;
    }

    return host->register_enemy(
        host->context,
        &dragonDefinition
    );
}
