#pragma once

#include <stdint.h>

#ifdef _WIN32
    #define GW_API __declspec(dllexport)
    #define GW_CALL __cdecl
#else
    #define GW_API __attribute__((visibility("default")))
    #define GW_CALL
#endif

#define GW_API_VERSION 2

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    void* context;

    void (GW_CALL *take_damage)(
        void* context,
        int32_t damage
    );

    void (GW_CALL *heal)(
        void* context,
        int32_t amount
    );

    void (GW_CALL *eat)(
        void* context,
        int32_t amount
    );

    void (GW_CALL *give_xp)(
        void* context,
        int32_t amount
    );

    void (GW_CALL *give_gold)(
        void* context,
        int32_t amount
    );

    int32_t (GW_CALL *get_health)(
        void* context
    );

    int32_t (GW_CALL *get_max_health)(
        void* context
    );

    int32_t (GW_CALL *get_hunger)(
        void* context
    );

    int32_t (GW_CALL *get_gold)(
        void* context
    );

} GWPlayerAPI;

typedef struct {
    uint32_t struct_size;

    const char* id;
    const char* name;
    const char* description;

    int32_t price;

    int (GW_CALL *on_use)(
        const GWPlayerAPI* player
    );

} GWItemDefinition;


typedef struct {
    uint32_t struct_size;

    const char* id;

    void* (GW_CALL *create)(void);

    void (GW_CALL *destroy)(void* enemy);

    const char* (GW_CALL *get_name)(void* enemy);

    int32_t (GW_CALL *get_health)(void* enemy);

    int32_t (GW_CALL *get_damage)(void* enemy);

    int32_t (GW_CALL *get_gold_reward)(void* enemy);

    void (GW_CALL *take_damage)(
        void* enemy,
        int32_t damage
    );

    void (GW_CALL *attack)(
        void* enemy,
        const GWPlayerAPI* player
    );

} GWEnemyDefinition;


typedef struct {
    uint32_t api_version;

    void* context;

    int (GW_CALL *register_enemy)(
        void* context,
        const GWEnemyDefinition* definition
    );

    int (GW_CALL *register_item)(
        void* context,
        const GWItemDefinition* definition
    );

} GWHostAPI;


// Every enemy plugin must export this function.
GW_API int GW_CALL gw_mod_init(
    const GWHostAPI* host
);

#ifdef __cplusplus
}
#endif