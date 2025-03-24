#include "GhostComponent.h"
#include "..\ecs\Manager.h"
#include <iostream>
#include <time.h>
#include <cstdlib>
#include <math.h>

GhostComponent::GhostComponent(int rad) 
	: _teleRadius(rad) {
}

GhostComponent::GhostComponent(int rad, int timer) 
	: _teleRadius(rad), _teleTimer(timer) {
}

GhostComponent::~GhostComponent() {
}


void GhostComponent::update() {

}

void GhostComponent::initComponent() { // coge su transform y el del player
	auto* _mngr = _ent->getMngr();
	_ghostTransform = _mngr->getComponent<Transform>(_ent);
	_playerTransform = _mngr->getComponent<Transform>(_mngr->getHandler(ecs::hdlr::PLAYER));
	assert(_ghostTransform != nullptr);
	assert(_playerTransform != nullptr);
}

void GhostComponent::teleport() {
	float srand(time(0));
	float rndnum = rand() % 361;



}
