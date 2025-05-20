#include "PickableCMP.h"
#include "..\ecs\Manager.h"


PickableCMP::PickableCMP(vector<float> upgradeval) : _upgradeValues(upgradeval) {

}


void PickableCMP::initComponent() {
	auto* _mngr = _ent->getMngr();

	_playerStats = _mngr->getComponent<EntityStat>(_mngr->getHandler(ecs::hdlr::PLAYER));
}

void PickableCMP::upgradePlayer() {
	for (int i = PickableCMP::UpgradeType::HealthTotal; 
		i <= PickableCMP::UpgradeType::AttackSpeedMult; i++) {
		switch (i)
		{
		case PickableCMP::HealthTotal:
			_playerStats->ChangeStat(_upgradeValues[i], EntityStat::Stat::HealthTotal);
			break;
		case PickableCMP::HealthCurrent:
			_playerStats->ChangeStat(_upgradeValues[i], EntityStat::Stat::HealthCurrent);
			break;
		case PickableCMP::MovementSpeedBase:
			_playerStats->ChangeStat(_upgradeValues[i], EntityStat::Stat::MovementSpeed);
			break;
		case PickableCMP::DamageBase:
			_playerStats->ChangeStat(_upgradeValues[i], EntityStat::Stat::Damage);
			break;
		case PickableCMP::AttackSpeedBase:
			_playerStats->ChangeStat(_upgradeValues[i], EntityStat::Stat::AttackSpeed);
			break;
		case PickableCMP::Shield:
			_playerStats->ChangeStat(_upgradeValues[i], EntityStat::Stat::Shield);
			break;
		case PickableCMP::MovementSpeedMult:
			_playerStats->ChangeMulti(_upgradeValues[i], EntityStat::Stat::MovementSpeed);
			break;
		case PickableCMP::DamageMult:
			_playerStats->ChangeMulti(_upgradeValues[i], EntityStat::Stat::Damage);
			break;
		case PickableCMP::AttackSpeedMult:
			_playerStats->ChangeMulti(_upgradeValues[i], EntityStat::Stat::AttackSpeed);
			break;
		}
	}
}

void PickableCMP::playerCollision(){
	upgradePlayer();
	_ent->getMngr()->setAlive(_ent, false);
}