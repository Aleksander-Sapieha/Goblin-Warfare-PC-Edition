/*
**  File: Goblin-Warfare-PC-Edition/src/Source.cpp
**
**  Creator: Aleksander Sapieha (sourceguy68@gmail.com)
**
**  Contributors:
**  Aleksander Sapieha (sourceguy68@gmail.com)
*/


#include <iostream>
#include <thread>
#include <chrono>
#include <random>
#include <list>
#include <algorithm> // for std::find
#include <string>
#include <vector>

#include "Player.hpp"
#include "Goblin.hpp"
#include "Source.hpp"
#include "save.hpp"
#include "ModLoader.hpp"
#include "ItemRegistry.hpp"

std::list<std::string> leatherBootsRecipe = { "Leather", "String" };

void printMenu() {
	std::cout << "Goblin Warfare: PC Edition (Alpha 4)" << std::endl;
	std::cout << "1. Play" << std::endl;
	std::cout << "2. Exit" << std::endl;
}

void pause(int seconds) {
	std::this_thread::sleep_for(std::chrono::seconds(seconds));
}

void clearScreen() {
	std::cout << "\033[2J\033[1;1H";
}

int main() {
	ModLoader modLoader;
	modLoader.loadMods();

	ItemRegistry itemRegistry;
	itemRegistry.registerVanillaItems();

	for (const Mod& mod : modLoader.getMods()) {
		itemRegistry.loadModItems(mod.directory, mod.name);
	}
	std::random_device rd;
	std::mt19937 gen(rd());
	printMenu();
	int choice;
	std::cin >> choice;
	if (choice == 1) {
	Player player = [&]() {
		try {
			return load();
		}
		catch (const std::runtime_error&) {
			Player newPlayer("player", 1000, 100, 1, 0, 100, 0, 0,
							10, 0, false, {}, 1000);

			std::cout << "You, a young kid, heard a legend about the holy scriptures..." << std::endl;
			pause(1);
			std::cout << "\"For in seven days Yahweh created the earth...\"" << std::endl;
			pause(1);
			std::cout << "But you doubted..." << std::endl;
			pause(1);
			std::cout << "You decided to try to get them." << std::endl;
			std::cout << "Goal: win a battle while holding The Holy Scriptures" << std::endl;
			system("pause");

			return newPlayer;
		}
	}();
		while(true){
			std::cout << "What do you want to do?" << std::endl;
			std::cout << "1. Fight" << std::endl;
			std::cout << "2. Buy" << std::endl;
			std::cout << "3. Use Item" << std::endl;
			std::cout << "4. Craft" << std::endl;
			std::cout << "5. Quit" << std::endl;
			std::cin >> choice;
			if (choice == 1) {
				int min = player.getLevel() - 1;
				int max = player.getLevel() + 3;
				std::uniform_int_distribution<> distrib(min, max);
				int randomNumber = distrib(gen);
				std::cout << "You found a goblin!" << std::endl;
				pause(1);
				Goblin goblin(randomNumber, randomNumber * 100, randomNumber * 50, randomNumber * 3, randomNumber * 10);
				while (true) {
					if (player.getScripturesStatus() == false) {
						player.takeDamage(goblin.getDamage());
					}
					else {
						std::cout << "You used the holy sciptures, the ultimate weapon in all of OmVille... You can see the goblin tremble in fear. The very next moment... He's dead." << std::endl;
					}
					goblin.takeDamage(player.getDamage());
					if (player.getHealth() <= 0) {
						break;
					}
					else if (goblin.getHealth() <= 0) {
						std::cout << "You killed the goblin!" << std::endl;
						player.loseHunger(10);
						pause(1);
						player.levelUp();
						player.addToInventory("String");
						std::cout << "Your remaining HP: " << player.getHealth() << std::endl;
						std::cout << "Your remaining Food Points: " << player.getHunger() << std::endl;
						pause(2);
						if (player.getHunger() <= 0) {
							player.takeDamage(10);
							std::cout << "You took damage because you're hungry" << std::endl;
							pause(1);
						}
						player.giveGold(goblin.getGoldValue());
						if (player.getScripturesStatus()) {
							std::cout << "What the shop keeped didn't tell you, is that using the scriptures against your enemies with curse you..." << std::endl;
							std::cout << "\"Do not seek revenge or bear a grudge against anyone among your people, but love your neighbor as yourself. I am Yahweh.\"" << std::endl;
							pause(5);
							player.takeScriptures();
							return 0;
						}
						break;
					}
					else {
						continue;
					}
				}
				if (player.getHealth() <= 0) {
					break;
				}
			}
			else if (choice == 2) {
				clearScreen();

				std::cout << "Gold: " << player.getGold() << std::endl;
				std::cout << "=== SHOP ===" << std::endl;

				const auto& shopItems = itemRegistry.getItems();

				for (size_t i = 0; i < shopItems.size(); i++) {
					std::cout << i + 1 << ". "
							<< shopItems[i].name
							<< " (" << shopItems[i].price << " Gold)"
							<< " [" << shopItems[i].mod << "]"
							<< std::endl;
				}

				std::cout << shopItems.size() + 1 << ". Exit" << std::endl;

				int purchase;
				std::cin >> purchase;

				if (purchase == static_cast<int>(shopItems.size() + 1)) {
					choice = 0;
					continue;
				}

				if (purchase < 1 ||
					purchase > static_cast<int>(shopItems.size())) {
					std::cout << "Invalid choice." << std::endl;
					pause(1);
					continue;
				}

				const GameItem& item = shopItems[purchase - 1];

				if (player.getGold() < item.price) {
					std::cout << "Not enough money!" << std::endl;
					pause(1);
					continue;
				}

				player.addToInventory(item.name);

				// Assumes giveGold adds its argument to the balance.
				player.takegold(item.price);

				std::cout << "Purchased " << item.name << "!" << std::endl;
				pause(1);
			}
			else if (choice == 3) {
				if (player.getInventory().empty()) {
					std::cout << "Your inventory is empty." << std::endl;
					pause(1);
					continue;
				}

				std::cout << "Your items:" << std::endl;

				for (const std::string& item : player.getInventory()) {
					std::cout << "- " << item << std::endl;
				}

				std::cout << "Which item do you want to use: ";

				std::string itemName;
				std::getline(std::cin >> std::ws, itemName);

				const GameItem* item = itemRegistry.getItem(itemName);

				if (!item) {
					std::cout << "Unknown item." << std::endl;
					pause(1);
					continue;
				}

				if (std::find(
					player.getInventory().begin(),
					player.getInventory().end(),
					itemName
				) == player.getInventory().end()) {
					std::cout << "You don't have that item." << std::endl;
					pause(1);
					continue;
				}

				if (item->effect == "heal") {
					int missingHealth = player.getMaxHealth() - player.getHealth();
					int healing = std::min(item->amount, missingHealth);

					if (healing <= 0) {
						std::cout << "Your health is already full." << std::endl;
						pause(1);
						continue;
					}

					player.heal(healing);
				}
				else if (item->effect == "food") {
					player.eat(item->amount);
				}
				else if (item->effect == "xp") {
					player.giveXp(item->amount);
				}
				else if (item->effect == "gold") {
					player.giveGold(item->amount);
				}
				else {
					std::cout << "This item has no usable effect." << std::endl;
					pause(1);
					continue;
				}

				player.removeFromInventory(itemName);

				std::cout << "Used " << itemName << "!" << std::endl;
				pause(1);
			}
			else if (choice == 4) {
				std::cout << "Craftable Items:" << std::endl;
				auto& inventory = player.getInventory();
				if ((std::find(inventory.begin(), inventory.end(), "Leather") != inventory.end())
					&& (std::find(inventory.begin(), inventory.end(), "String") != inventory.end())) {
					std::cout << "- Leather Boots" << std::endl;
				}
				std::string answer;
				std::cout << "Enter the name of the item you wanna craft, or type exit: ";
				std::getline(std::cin >> std::ws, answer);
				if (answer == "exit") {
					continue;
				}
				else if (answer == "Leather Boots") {
					// check materials and craft
					if ((std::find(inventory.begin(), inventory.end(), "Leather") != inventory.end())
						&& (std::find(inventory.begin(), inventory.end(), "String") != inventory.end())) {
						// remove materials and add crafted item
						player.removeFromInventory("Leather");
						player.removeFromInventory("String");
						player.addToInventory("Leather Boots");
						std::cout << "You crafted Leather Boots." << std::endl;
						pause(1);
						continue;
					}
					else {
						std::cout << "You don't have the required materials." << std::endl;
						pause(1);
						continue;
					}
				}
			}
			else if (choice == 5) {
				std::cout << "Saving..." << std::endl;
				save(player);
				return 0;
			}
		}
	}
	else if (choice == 2) {
		return 0;
	}
	return 0;
}