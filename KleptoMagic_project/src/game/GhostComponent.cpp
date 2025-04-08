#define _USE_MATH_DEFINES

#include "GhostComponent.h"
#include "..\ecs\Manager.h"
#include <iostream>
#include <cstdlib>
#include <math.h>

GhostComponent::GhostComponent(int rad, int timer) 
	: _teleRadius(rad), _cooldownTime(timer) {
	std::uniform_real_distribution<float> rndDistributor(1.2f, 2*M_PI);
	std::chrono::steady_clock::time_point lastAttackTime = std::chrono::steady_clock::now();
}

GhostComponent::~GhostComponent() {
}


void GhostComponent::update() {
	auto now = std::chrono::steady_clock::now();
	float elapsedTime = std::chrono::duration<float>(now - lastAttackTime).count();

	if (elapsedTime >= _cooldownTime)
	{
		std::cout << "ataqueFantasma!";
		lastAttackTime = now;
		teleportRndPosition();
	}

}

void GhostComponent::initComponent() { // Gets it's transform and target's, and the timer
	auto* _mngr = _ent->getMngr();
	_ghostTransform = _mngr->getComponent<Transform>(_ent);
	_targetTransform = _mngr->getComponent<Transform>(_mngr->getHandler(ecs::hdlr::PLAYER));
	assert(_ghostTransform != nullptr);
	assert(_targetTransform != nullptr);
}

void GhostComponent::teleportRndPosition() {
	Vector2D result;
	float rndAng = rndDistributor(rndGenerator);
	result.setX(_targetTransform->getPos().getX() * cos(rndAng));
	result.setY(_targetTransform->getPos().getY() * sin(rndAng));

	_ghostTransform->getPos() = result;
}
