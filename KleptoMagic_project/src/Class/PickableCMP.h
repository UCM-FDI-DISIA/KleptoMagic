#ifndef PICK_COMPONENT_C
#define PICK_COMPONENT_C

#include "../ecs/Component.h"
#include "../Class/Transform.h"
#include "../Class/EntityStat.h"


#pragma once
class PickableCMP : public ecs::Component
{
private:
	EntityStat* _playerStats;	//puntero a los stats del player

	EntityStat::Stat _upgradeType;	// indica que stat es afectado
	bool _isMult = false,	// indica la adicion es para el multiplicador o el sumador
		_expirable = false;			// indica si el upgrade desaparece pasado un tiempo
	float _value;
	
	void upgradePlayer();

public:
	__CMPID_DECL__(ecs::cmp::PICKABLECMP);

	PickableCMP(EntityStat::Stat upgradeType, float value, bool isMult);
	void initComponent() override;
	void collideWithPlayer();


};

#endif // PICK_COMPONENT_C