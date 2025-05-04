#include "PickableCMP.h"
#include "..\ecs\Manager.h"


PickableCMP::PickableCMP(EntityStat::Stat upgradeType, float value, bool isMult) 
	: _upgradeType(upgradeType), _isMult(isMult), _value(value) {
}


void PickableCMP::initComponent() {
	auto* _mngr = _ent->getMngr();

	_playerStats = _mngr->getComponent<EntityStat>(_mngr->getHandler(ecs::hdlr::PLAYER));
}

void PickableCMP::playerColision() {
	/*if(_ismult){
		_playerstats->changemulti(_value, _upgradetype);
	}
	else {
		_playerstats->changeflat(_value, _upgradetype);
	}*/

	delete _ent;
}