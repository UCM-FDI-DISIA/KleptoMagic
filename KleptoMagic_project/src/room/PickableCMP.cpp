#include "PickableCMP.h"
#include "..\ecs\Manager.h"


void PickableCMP::initComponent() {
	auto* _mngr = _ent->getMngr();

	_playerStats = _mngr->getComponent<EntityStat>(_mngr->getHandler(ecs::hdlr::PLAYER));
	_itemStats = _mngr->getComponent<ObjectInfo>(_ent)->getItemInfo();
}

void PickableCMP::upgradePlayer() {
	_playerStats->ChangeStat(_itemStats.healthMax, EntityStat::Stat::HealthTotal);
	_playerStats->ChangeStat(_itemStats.heal, EntityStat::Stat::HealthCurrent);
	_playerStats->ChangeStat(_itemStats.movementSpeed, EntityStat::Stat::MovementSpeed);
	_playerStats->ChangeStat(_itemStats.damage, EntityStat::Stat::Damage);
	_playerStats->ChangeStat(_itemStats.attackSpeed, EntityStat::Stat::AttackSpeed);
	_playerStats->ChangeStat(_itemStats.shield, EntityStat::Stat::Shield);
	_playerStats->ChangeMulti(_itemStats.movementSpeedMult, EntityStat::Stat::MovementSpeed);
	_playerStats->ChangeMulti(_itemStats.damageMult, EntityStat::Stat::Damage);
	_playerStats->ChangeMulti(_itemStats.attackSpeedMult, EntityStat::Stat::AttackSpeed);
}

void PickableCMP::playerCollision(){
	upgradePlayer();
	_ent->getMngr()->setAlive(_ent, false);
}