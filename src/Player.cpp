/*
**  File: Goblin-Warfare-PC-Edition/src/Player.cpp
**
**  Creator: Aleksander Sapieha (sourceguy68@gmail.com)
**
**  Contributors:
**  Aleksander Sapieha (sourceguy68@gmail.com)
*/

#include "Player.hpp"

Player::Player(std::string name, int health, int hunger, int level, int xp, int xpTilNextLevel, int score, int gold, int damage, float armor, bool hasScriptures,
	std::list<std::string> inventory, int maxHealth)
	: name(name), health(health), hunger(hunger), level(level), xp(xp), xpTilNextLevel(xpTilNextLevel), score(score), gold(gold), damage(damage), armor(armor),
	hasScriptures(hasScriptures), inventory(inventory), maxHealth(maxHealth) {
};

int Player::getHealth() const {
	return health;
}

std::string Player::getName() const {
	return name;
}

int Player::getXp() const {
	return xp;
}

int Player::getXpTilNextLevel() const {
	return xpTilNextLevel;
}

int Player::getScore() const {
	return score;
}

int Player::getHunger() const {
	return hunger;
}

int Player::getDamage() const {
	return damage;
}

int Player::getGold()const  {
	return gold;
}

int Player::getLevel() const {
	return level;
}

void Player::takeDamage(int points) {
	health = health - points * (1 - armor);
}

void Player::heal(int points) {
	health = health + points;
}

void Player::eat(int points) {
	hunger = hunger + points;
}

void Player::loseHunger(int points) {
	hunger = hunger - points;
}

void Player::giveGold(int points) {
	gold = gold + points;
}

void Player::levelUp() {
	if (xp >= xpTilNextLevel) {
		xpTilNextLevel = xpTilNextLevel * 2;
		level++;
		maxHealth = maxHealth + 10;
		damage = damage + 20;
	}
}

void Player::takegold(int points) {
	gold = gold - points;
}

float Player::getArmor() const {
	return armor;
}

void Player::giveArmor(float points) {
	armor = armor + points;
	if (armor >= 1.0f) {
		armor = 0.9f;
	}
}

void Player::takeArmor(float points) {
	armor = armor - points;
	if (armor < 0.0f) {
		armor = 0.0f;
	}
}

bool Player::getScripturesStatus() const {
	return hasScriptures;
}

void Player::giveScriptures() {
	hasScriptures = true;
}

void Player::takeScriptures() {
	hasScriptures = false;
}

void Player::giveXp(int points) {
	xp = xp + points;
}

const std::list<std::string>& Player::getInventory() const {
    return inventory;
}

void Player::addToInventory(std::string item) {
	inventory.push_front(item);
}

void Player::removeFromInventory(const std::string& item) {
	inventory.remove(item);
}

void Player::addMaxHealth(int points) {
	maxHealth = maxHealth + points;
}

int Player::getMaxHealth() const {
	return maxHealth;
}