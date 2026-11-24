/*
**  File: Goblin-Warfare-PC-Edition/src/Goblin.hpp
**
**  Creator: Aleksander Sapieha (sourceguy68@gmail.com)
**
**  Contributors:
**  Aleksander Sapieha (sourceguy68@gmail.com)
*/

#pragma once

#include "Player.hpp"

class Goblin {
private:
	int health;
	int level;
	int attackPower;
	int scoreValue;
	int goldReward;

public:
	Goblin(int level, int scoreValue, int health, int attackPower, int goldReward);
	void attack(Player player);
	int getHealth();
	void takeDamage(int points);
	int getGoldValue();
	int getDamage();
};