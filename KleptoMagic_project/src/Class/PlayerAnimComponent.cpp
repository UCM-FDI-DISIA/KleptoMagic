#include "PlayerAnimComponent.h"
#include <iostream>
using namespace std;

PlayerAnimComponent::PlayerAnimComponent() {
	isWalking = false;
	isFacingRight = false;
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
		_img->setStartingFrame(startFrame);
		_img->setFrame(startFrame + 1);
		_img->setNumFrames(4);
	}
	else {
		_img->setStartingFrame(startFrame);
		_img->setFrame(startFrame);
		_img->setNumFrames(1);
	}
	isWalking = !isWalking;
}

void PlayerAnimComponent::playDeath() {
	_img->setStartingFrame(deathFrame);
	_img->setFrame(deathFrame);
	_img->setNumFrames(1);
}