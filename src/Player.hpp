/*
**  File: Goblin-Warfare-PC-Edition/src/Player.hpp
**
**  Creator: Aleksander Sapieha (sourceguy68@gmail.com)
**
**  Contributors:
**  Aleksander Sapieha (sourceguy68@gmail.com)
*/

#pragma once

#include <iostream>
#include <list>
class Player {
private:
	std::string name;
	int health;
	int hunger;
	int level;
	int xp;
	int xpTilNextLevel;
	int score;
	int gold;
	int damage;
	int armor;
	bool hasScriptures;
	std::list<std::string> inventory;
	int maxHealth;

public:
	Player(std::string name, int health, int hunger, int level, int xp, int xpTilNextLevel, int score, int gold, int damage, float armor, bool hasScriptures,
		std::list<std::string> inventory, int maxHealth);
	void takeDamage(int points);
	std::string getName() const;
	int getHealth() const;
	int getGold() const;
	int getXp() const;
	int getXpTilNextLevel() const;
	int getScore() const;
	int getHunger() const;
	int getDamage() const;
	void heal(int points);
	void eat(int points);
	void loseHunger(int points);
	void giveGold(int points);
	void levelUp();
	int getLevel() const;
	void takegold(int points);
	float getArmor() const;
	void giveArmor(float points);
	void takeArmor(float points);
	bool getScripturesStatus() const;
	void giveScriptures();
	void takeScriptures();
	void giveXp(int points);
	const std::list<std::string>& getInventory() const;
	void addToInventory(std::string item);
	void removeFromInventory(const std::string& item);
	int getMaxHealth() const;
	void addMaxHealth(int points);
};