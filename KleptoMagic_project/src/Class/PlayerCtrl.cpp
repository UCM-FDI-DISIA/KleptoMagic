#include "PlayerCtrl.h"
#include "../sdlutils/NewInputHandler.h"
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
//#include <iostream>


void PlayerCtrl::initComponent() { //falta get entity stats speed
	auto*_mngr = _ent->getMngr();
	_tr = _mngr->getComponent<Transform>(_ent);
	assert(_tr != nullptr);
}
//como comentaba en el handleEvent del player cpp o muevo la speed de aqui a alli o
//o hago un segundo metodo Move que no aplique speed para el knockback pero... creo que mejor lo otro
void PlayerCtrl::Move(Vector2D movementVector, float speed) {
	_tr->getVel() = movementVector * speed;
}

void PlayerCtrl::update() {
	float speed = 4; //get entity Stats speed
	Move(NewInputHandler::Instance()->getMovementVector(), speed);
}