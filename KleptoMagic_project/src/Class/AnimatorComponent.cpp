#include "AnimatorComponent.h"
#include <iostream>
using namespace std;

void AnimatorComponent::initComponent() {
	auto* _mngr = _ent->getMngr();
	_tr = _mngr->getComponent<Transform>(_ent);
	assert(_tr != nullptr);
	_img = _mngr->getComponent<ImageWithFrames>(_ent);
	assert(_img != nullptr);
}

void AnimatorComponent::toggleFlip() {
	if (!isFacingRight) {
		_img->setFlip(true);
	}
	else {
		_img->setFlip(false);
	}
	isFacingRight = !isFacingRight;
}

void AnimatorComponent::playDeath() {
	_img->setStartingFrame(deathFrame);
	_img->setFrame(deathFrame);
	_img->setNumFrames(60);
}