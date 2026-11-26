/*
**  File: Goblin-Warfare-PC-Edition/src/save.hpp
**
**  Creator: Aleksander Sapieha (sourceguy68@gmail.com)
**
**  Contributors:
**  Aleksander Sapieha (sourceguy68@gmail.com)
*/

#pragma once

#include "SDD/sdd.h"

Value serializePlayer(const Player& player);

Player deserializePlayer(const Value& data);

void save(Player player);

Player load();