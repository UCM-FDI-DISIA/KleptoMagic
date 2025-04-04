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
	Vector2D movementVector = { 0, 0 };
	float speed = 1; //get entity Stats speed

	if (NewInputHandler::Instance()->isActionHeld(Action::MOVE_RIGHT)) {
		movementVector = { 1, 0 };
	}
	else if (NewInputHandler::Instance()->isActionHeld(Action::MOVE_LEFT)) {
		movementVector = { -1, 0 };
	}
	else if (NewInputHandler::Instance()->isActionHeld(Action::MOVE_UP)) {
		movementVector = { 0, -1 };
	}
	else if (NewInputHandler::Instance()->isActionHeld(Action::MOVE_DOWN)) {
		movementVector = { 0, 1 };
	}
	Move(movementVector, speed);
}