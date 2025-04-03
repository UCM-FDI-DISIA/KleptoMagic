#pragma once
#include <SDL.h>
#include "../ecs/Component.h"
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"

class ObjectInfo : public ecs::Component {
private:

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
	Stats stat;

public:
	__CMPID_DECL__(ecs::cmp::PLAYERCTRL);

	void initItem(std::string n, float h, float sh, float hM,
		float mS, float mSM,
		float d, float dM,
		float aS, float aSM, Special sEf);

	Stats getItemInfo() {
		return stat;
	}


};