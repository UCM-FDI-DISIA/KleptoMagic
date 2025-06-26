#ifndef PICK_COMPONENT_C
#define PICK_COMPONENT_C

#include "../ecs/Component.h"
#include "../ecs/Transform.h"
#include "../ecs/EntityStat.h"
#include "../room/ObjectInfo.h"
#include <vector>

using namespace std;

#pragma once
class PickableCMP : public ecs::Component
{
private:
	enum UpgradeType
	{
		HealthTotal,
		HealthCurrent,
		MovementSpeedBase,
		DamageBase,
		AttackSpeedBase,
		Shield,
		MovementSpeedMult,
		DamageMult,
		AttackSpeedMult
	};

	EntityStat* _playerStats;	//puntero a los stats del player

	bool _expirable = false;	// indica si el upgrade desaparece pasado un tiempo
	vector<float> _upgradeValues;	// vector que incluye los valores de todos upgrades
	
	void upgradePlayer();

public:
	__CMPID_DECL__(ecs::cmp::PICKABLECMP);

	PickableCMP(vector<float> upgradeValues);
	PickableCMP();	// generico para debug, no deberia ser usado

	void initComponent() override;
	void playerCollision();

};

#endif // PICK_COMPONENT_C