#include "PickableCMP.h"
#include "..\ecs\Manager.h"


PickableCMP::PickableCMP(EntityStat::Stat upgradeType, float value, bool isMult) 
	: _upgradeType(upgradeType), _isMult(isMult), _value(value) {
}


void PickableCMP::initComponent() {
	auto* _mngr = _ent->getMngr();

	_playerStats = _mngr->getComponent<EntityStat>(_mngr->getHandler(ecs::hdlr::PLAYER));
}

void PickableCMP::upgradePlayer() {
	if(_isMult){
		_playerStats->ChangeMulti(_value, _upgradeType);
	}
	else {
		_playerStats->ChangeFlat(_value, _upgradeType);
	}
}

void PickableCMP::collideWithPlayer(){
	upgradePlayer();
	// destruirse y el restos


}