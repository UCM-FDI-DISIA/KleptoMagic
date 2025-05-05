#include "AnimatorComponent.h"
#include <iostream>
using namespace std;

void AnimatorComponent::initComponent() {
	auto* _mngr = _ent->getMngr();
	_tr = _mngr->getComponent<Transform>(_ent);
	assert(_tr != nullptr);
	_plrImg = _mngr->getComponent<ImageWithFrames>(_ent);
	assert(_plrImg != nullptr);
}

void AnimatorComponent::toggleFlip() {
	if (!isFacingRight) {
		_plrImg->setFlip(true);
	}
	else {
		_plrImg->setFlip(false);
	}
	isFacingRight = !isFacingRight;
}