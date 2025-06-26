#include "ItemStorage.h"
#include <fstream>
#include <sstream>
#include <iostream>

Special ItemStorage::parseSpecial(const std::string& str) {
	if (str == "none") return Special::none;
	if (str == "explode") return Special::explode;
	if (str == "divide") return Special::divide;
	if (str == "pierce") return Special::pierce;
	return Special::none;
}

ItemStorage::ItemStorage(const std::string& filePath) {
	std::ifstream file(filePath);
	if (!file.is_open()) {
		std::cerr << "Error al abrir el archivo: " << filePath << std::endl;
		return;
	}

	std::string line;
	while (std::getline(file, line)) {
		std::istringstream iss(line);
		Stats s;
		std::string specialStr;

		if (!(iss >> s.name >> s.heal >> s.shield >> s.healthMax >>
			s.movementSpeed >> s.movementSpeedMult >>
			s.damage >> s.damageMult >>
			s.attackSpeed >> s.attackSpeedMult >> specialStr)) {
			std::cerr << "Error al leer l�nea: " << line << std::endl;
			continue;
		}

		s.sEffect = parseSpecial(specialStr);
		items.push_back(s);
	}

	file.close();
}

Stats ItemStorage::getRandomItem() const {
	if (items.empty()) {
		std::cerr << "No hay �tems cargados." << std::endl;
		return Stats{};
	}

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distr(0, items.size() - 1);
	return items[distr(gen)];
}