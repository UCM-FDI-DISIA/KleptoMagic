#include "PlayerAnimComponent.h"

void PlayerAnimComponent::initComponent() { //falta get entity stats speed
	auto* _mngr = _ent->getMngr();
	_tr = _mngr->getComponent<Transform>(_ent);
	assert(_tr != nullptr);
	_plrImg = _mngr->getComponent<ImageWithFrames>(_ent);
	assert(_plrImg != nullptr);
	walkingFrame = 0;
	walking = false;
}