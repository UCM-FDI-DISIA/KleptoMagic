#include "GhostComponent.h"
#include "..\ecs\Manager.h"

GhostComponent::GhostComponent(int rad) 
	: _teleRadius(rad) {
}

GhostComponent::GhostComponent(int rad, int timer) 
	: _teleRadius(rad), _teleTimer(timer) {
}

GhostComponent::~GhostComponent() {
}

void GhostComponent::initComponent() {
	auto* _mngr = _ent->getMngr();
	_ghostTransform = _mngr->getComponent<Transform>(_ent);
	_playerTransform = _mngr->getComponent<Transform>(_mngr->getHandler(ecs::hdlr::PLAYER));
	//_playerTransform = _mngr->getEntities()
	assert(_tr != nullptr);
}

void GhostComponent::teleport() {

}

void GhostComponent::update() {

}

