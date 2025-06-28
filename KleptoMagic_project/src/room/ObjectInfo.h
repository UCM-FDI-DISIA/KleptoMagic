#pragma once
#include <SDL.h>
#include "../ecs/Component.h"
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"

enum Special { none, explode, divide, pierce };


struct Stats {
	//nombre del item
	std::string name;

	//vida related
	float heal;
	float shield;
	float healthMax;

	//Velocidad de movimiento 
	float movementSpeed;
	float movementSpeedMult;

	//Daño
	float damage;
	float damageMult;

	//Velocidad de ataque
	float attackSpeed;
	float attackSpeedMult;

	//efecto especial
	Special sEffect;
};

class ObjectInfo : public ecs::Component {
private:
	Stats _stats;

public:
	__CMPID_DECL__(ecs::cmp::PLAYERCTRL);

	ObjectInfo(Stats stats) : _stats(stats) {};

	void initComponent() override {};

	Stats getItemInfo() {
		return _stats;
	}
};