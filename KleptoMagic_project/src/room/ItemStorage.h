#pragma once

#include <string>
#include <vector>
#include <random>
#include "../room/ObjectInfo.h"


class ItemStorage {
private:
	std::vector<Stats> items;

	Special parseSpecial(const std::string& str);

public:
	ItemStorage(const std::string& filePath);

	// Devuelve un ítem aleatorio
	Stats getRandomItem() const;
};