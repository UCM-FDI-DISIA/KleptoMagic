#pragma once

#include <string>
#include <vector>
#include <random>

enum class Special { none, explode, divide, pierce };

struct Stats {
	std::string name;
	float heal;
	float shield;
	float healthMax;
	float movementSpeed;
	float movementSpeedMult;
	float damage;
	float damageMult;
	float attackSpeed;
	float attackSpeedMult;
	Special sEffect;
};

class ItemStorage {
private:
	std::vector<Stats> items;

	Special parseSpecial(const std::string& str);

public:
	ItemStorage(const std::string& filePath);

	// Devuelve un ítem aleatorio
	Stats getRandomItem() const;
};