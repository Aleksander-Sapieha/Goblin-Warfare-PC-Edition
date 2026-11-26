# Goblin Warfare Enemy Modding API

**API Name:** GW Enemy API
**API Version:** 1
**Header:** `GWEnemyAPI.hpp`
**Implementation:** `PluginLoader.cpp`
**Language:** C-compatible ABI

## 1. Overview

The Goblin Warfare Enemy Modding API allows third-party developers to create custom enemies and register them with the game through native plugins.

The API uses a C-compatible Application Binary Interface (ABI), allowing plugins to be developed in C, C++, or other languages capable of producing compatible native libraries.

Features include:

- Custom enemy creation and destruction.
- Custom enemy names and statistics.
- Custom enemy damage handling.
- Custom enemy attack behavior.
- Access to selected player functionality.
- Dynamic plugin loading.
- Runtime enemy registration.

## 2. Platform Compatibility

| Platform | Library format |
| -------- | -------------- |
| Windows  | `.dll`         |
| Linux    | `.so`          |

### API macros

**`GW_API`**

Controls symbol visibility.

- Windows: `__declspec(dllexport)`
- Other platforms: `__attribute__((visibility("default")))`

**`GW_CALL`**

Defines the calling convention.

- Windows: `__cdecl`
- Other platforms: Default platform calling convention.

**`GW_API_VERSION`**

Defines the current API version.

Current value: `1`.

## 3. GWPlayerAPI

Provides plugins with access to selected player functionality.

```c
typedef struct {
    void* context;

    void (GW_CALL *take_damage)(
        void* context,
        int32_t damage
    );

} GWPlayerAPI;
```

### Members

| Member        | Type             | Description                   |
| ------------- | ---------------- | ----------------------------- |
| `context`     | `void*`          | Player context.               |
| `take_damage` | Function pointer | Applies damage to the player. |

### `take_damage`

```c
void (GW_CALL *take_damage)(
    void* context,
    int32_t damage
);
```

Requests that the player receives the specified amount of damage.

**Parameters:**

- `context`: Player context supplied by the game.
- `damage`: Amount of damage to apply.

## 4. GWEnemyDefinition

Defines an enemy that can be registered with Goblin Warfare.

```c
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
```

### Members

| Member            | Type             | Description                       |
| ----------------- | ---------------- | --------------------------------- |
| `struct_size`     | `uint32_t`       | Size of the structure in bytes.   |
| `id`              | `const char*`    | Unique enemy identifier.          |
| `create`          | Function pointer | Creates an enemy instance.        |
| `destroy`         | Function pointer | Destroys an enemy instance.       |
| `get_name`        | Function pointer | Returns the enemy's display name. |
| `get_health`      | Function pointer | Returns enemy health.             |
| `get_damage`      | Function pointer | Returns enemy attack damage.      |
| `get_gold_reward` | Function pointer | Returns the gold reward.          |
| `take_damage`     | Function pointer | Applies damage to the enemy.      |
| `attack`          | Function pointer | Executes enemy attack behavior.   |

### 4.1. Structure size

`struct_size` must equal `sizeof(GWEnemyDefinition)` expected by the host.

The host rejects definitions with incompatible structure sizes.

### 4.2. Enemy identification

The `id` field identifies an enemy.

Enemy IDs must be unique across all currently registered plugins.

Registering an enemy with an existing ID will fail.

### 4.3. create

```c
void* (GW_CALL *create)(void);
```

Creates a new enemy instance.

**Returns:** Pointer to the newly created enemy instance.

### 4.4. destroy

```c
void (GW_CALL *destroy)(void* enemy);
```

Destroys an enemy instance.

**Parameters:**

- `enemy`: Pointer to the enemy instance.

The plugin is responsible for releasing resources associated with the instance.

### 4.5. get_name

```c
const char* (GW_CALL *get_name)(void* enemy);
```

Returns the enemy's display name.

**Returns:** Null-terminated string containing the enemy name.

### 4.6. get_health

```c
int32_t (GW_CALL *get_health)(void* enemy);
```

Returns the enemy's health.

### 4.7. get_damage

```c
int32_t (GW_CALL *get_damage)(void* enemy);
```

Returns the enemy's attack damage.

### 4.8. get_gold_reward

```c
int32_t (GW_CALL *get_gold_reward)(void* enemy);
```

Returns the amount of gold awarded when the enemy is defeated.

### 4.9. take_damage

```c
void (GW_CALL *take_damage)(
    void* enemy,
    int32_t damage
);
```

Applies damage to an enemy.

**Parameters:**

- `enemy`: Pointer to the enemy instance.
- `damage`: Amount of damage to apply.

### 4.10. attack

```c
void (GW_CALL *attack)(
    void* enemy,
    const GWPlayerAPI* player
);
```

Executes the enemy's attack behavior.

**Parameters:**

- `enemy`: Pointer to the enemy instance.
- `player`: Pointer to the player API.

The plugin can use the player API to interact with the player.

## 5. GWHostAPI

Provides plugins with access to functionality exposed by the game.

```c
typedef struct {
    uint32_t api_version;

    void* context;

    int (GW_CALL *register_enemy)(
        void* context,
        const GWEnemyDefinition* definition
    );

} GWHostAPI;
```

### Members

| Member           | Type             | Description            |
| ---------------- | ---------------- | ---------------------- |
| `api_version`    | `uint32_t`       | Host API version.      |
| `context`        | `void*`          | Host-provided context. |
| `register_enemy` | Function pointer | Registers an enemy.    |

### 5.1. register_enemy

```c
int (GW_CALL *register_enemy)(
    void* context,
    const GWEnemyDefinition* definition
);
```

Registers an enemy definition with the game.

**Parameters:**

- `context`: Host context provided by the game.
- `definition`: Pointer to the enemy definition.

**Return values:**

| Value | Meaning                  |
| ----- | ------------------------ |
| `1`   | Registration successful. |
| `0`   | Registration failed.     |

Registration can fail for the following reasons:

- Invalid host context.
- Null enemy definition.
- Incompatible structure size.
- Missing required fields or callbacks.
- Duplicate enemy ID.

All function pointers in `GWEnemyDefinition` are mandatory.

## 6. Plugin Initialization

Every enemy plugin must export the following function:

```c
GW_API int GW_CALL gw_mod_init(
    const GWHostAPI* host
);
```

The game searches for this symbol when loading a native plugin.

### Parameters

- `host`: Pointer to the host API provided by Goblin Warfare.

### Return values

| Value   | Meaning                    |
| ------- | -------------------------- |
| Nonzero | Initialization successful. |
| `0`     | Initialization failed.     |

If initialization fails, all enemies registered by that plugin during initialization are discarded.

### Example

```cpp
#include "GWEnemyAPI.hpp"

class Dragon {
public:
    int health = 1000;
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
    return static_cast<Dragon*>(enemy)->health;
}

static int32_t GW_CALL getDragonDamage(void*) {
    return 100;
}

static int32_t GW_CALL getDragonGold(void*) {
    return 500;
}

static void GW_CALL dragonTakeDamage(
    void* enemy,
    int32_t damage
) {
    static_cast<Dragon*>(enemy)->health -= damage;
}

static void GW_CALL dragonAttack(
    void* enemy,
    const GWPlayerAPI* player
) {
    if (player && player->take_damage) {
        player->take_damage(player->context, 100);
    }
}

extern "C" {

GW_API int GW_CALL gw_mod_init(
    const GWHostAPI* host
) {
    if (!host || !host->register_enemy) {
        return 0;
    }

    GWEnemyDefinition dragon{};

    dragon.struct_size = sizeof(GWEnemyDefinition);
    dragon.id = "dragon";

    dragon.create = createDragon;
    dragon.destroy = destroyDragon;

    dragon.get_name = getDragonName;
    dragon.get_health = getDragonHealth;
    dragon.get_damage = getDragonDamage;
    dragon.get_gold_reward = getDragonGold;

    dragon.take_damage = dragonTakeDamage;
    dragon.attack = dragonAttack;

    return host->register_enemy(
        host->context,
        &dragon
    );
}

}
```

## 7. Plugin Loader

The game uses a native plugin loader to load and manage enemy plugins.

### Loading plugins

The loader uses:

- `LoadLibraryA()` on Windows.
- `dlopen()` on Linux.

Plugins are loaded using immediate symbol resolution and local symbol visibility on Linux (`RTLD_NOW | RTLD_LOCAL`).

The loader searches for the exported `gw_mod_init` function.

A plugin is rejected if:

- The native library cannot be loaded.
- The `gw_mod_init` symbol is missing.
- Initialization returns zero.

### Enemy registration

When an enemy is registered, the loader:

1. Validates the provided structure.
2. Checks that all required fields are present.
3. Checks for duplicate enemy IDs.
4. Copies the enemy ID into its own `std::string`.
5. Copies the enemy definition into its internal registry.
6. Associates successfully registered enemies with their plugin after initialization succeeds.

The loader does not retain the original pointer to the `GWEnemyDefinition` structure.

However, the pointers contained inside that structure are copied as pointer values, not deep-copied.

### Unloading plugins

The loader provides an `unloadAll()` operation.

Its behavior:

1. Clears the registered enemy definitions.
2. Unloads all native plugin libraries.
3. Clears the plugin registry.

Registered callbacks must not be used after their corresponding plugin has been unloaded.

## 8. Compatibility and Development Requirements

- Plugins must export `gw_mod_init`.
- Plugins must use the correct calling convention.
- Plugins must be compiled for the same architecture as the game.
- All required enemy callbacks must be implemented.
- Enemy IDs must be unique.
- Plugin-owned callback code must remain available while registered.
- Strings referenced by the enemy definition must remain valid while needed by the host.
- Enemy instances must be compatible with their plugin's `create` and `destroy` callbacks.

The host exposes its API version through `GWHostAPI.api_version`.

The current API version is `1`.

Plugins should check this value during initialization.

## 9. Error Messages

The plugin loader may produce the following messages:

| Message                              | Description                                   |
| ------------------------------------ | --------------------------------------------- |
| `Failed to load`                     | Native library loading failed.                |
| `Missing gw_mod_init`                | Required initialization symbol was not found. |
| `Initialization failed`              | Plugin initialization returned zero.          |
| `Incompatible enemy structure size!` | Structure size does not match.                |
| `Invalid enemy definition!`          | One or more required fields are missing.      |
| `Duplicate enemy ID`                 | Another registered enemy uses the same ID.    |
| `Registered enemy`                   | Enemy registration succeeded.                 |
| `Loaded`                             | Plugin loading succeeded.                     |

## 10. Security Considerations

Native plugins execute code with the permissions of the game process.

Only load plugins from developers and sources you trust.

The API provides an interface for plugin integration, not a sandbox for untrusted native code.

---

**End of GW Enemy API Documentation**
