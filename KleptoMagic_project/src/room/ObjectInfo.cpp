#include "ObjectInfo.h"

void ObjectInfo::initItem(std::string n, float h, float sh, float hM,
	float mS, float mSM,
	float d, float dM,
	float aS, float aSM, Special sEf) {

	stat.name = n;

	stat.heal = h;
	stat.healthMax = hM;
	stat.shield = sh;

	stat.movementSpeed = mS;
	stat.movementSpeedMult = mSM;

	stat.damage = d;
	stat.damageMult = dM;

	stat.attackSpeed = aS;
	stat.attackSpeedMult = aSM;

	stat.sEffect = sEf;
}