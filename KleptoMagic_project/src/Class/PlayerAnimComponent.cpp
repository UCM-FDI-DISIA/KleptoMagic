#include "PlayerAnimComponent.h"
#include <iostream>
using namespace std;

PlayerAnimComponent::PlayerAnimComponent(int startF) : startFrame(startF), deathFrame(startF + 4) {
	isWalking = false;
	isFacingRight = false;
}

void PlayerAnimComponent::initComponent() {
	auto* _mngr = _ent->getMngr();
	_tr = _mngr->getComponent<Transform>(_ent);
	assert(_tr != nullptr);
	_plrImg = _mngr->getComponent<ImageWithFrames>(_ent);
	assert(_plrImg != nullptr);
}

void PlayerAnimComponent::update() {
	// check speed to see if it's walking or not
	int velX = _tr->getVel().getX();
	int velY = _tr->getVel().getY();
	bool isCurrentlyWalking = (velX != 0 || velY != 0);
	bool isCurrentlyMovingSideways = (velX != 0);
	bool isCurrentlyMovingRight = (velX < 0);
	//cout << velX << "|" << velY << "   " << isCurrentlyWalking << "|" << isWalking << "   " << isCurrentlyMovingRight << "|" <<isFacingRight << endl;

	if (!isWalking && isCurrentlyWalking) {
		toggleWalkingAnim();
	}
	else if (isWalking && !isCurrentlyWalking) {
		toggleWalkingAnim();
	}

	if (isFacingRight && !isCurrentlyMovingRight && isCurrentlyMovingSideways) {
		toggleFlip();
	}
	if (!isFacingRight && isCurrentlyMovingRight && isCurrentlyMovingSideways) {
		toggleFlip();
	}
}

void PlayerAnimComponent::toggleWalkingAnim() {
	if (!isWalking) {
		_plrImg->setStartingFrame(startFrame);
		_plrImg->setFrame(startFrame + 1);
		_plrImg->setNumFrames(4);
	}
	else {
		_plrImg->setStartingFrame(startFrame);
		_plrImg->setFrame(startFrame);
		_plrImg->setNumFrames(1);
	}
	isWalking = !isWalking;
}

void PlayerAnimComponent::toggleFlip() {
	if (!isFacingRight) {
		_plrImg->setFlip(true);
	}
	else {
		_plrImg->setFlip(false);
	}
	isFacingRight = !isFacingRight;
}

void PlayerAnimComponent::playDeath() {
	_plrImg->setStartingFrame(deathFrame);
	_plrImg->setFrame(deathFrame);
	_plrImg->setNumFrames(1);
}