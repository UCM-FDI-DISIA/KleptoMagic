#include "MovementCtrl.h"
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
//#include <iostream>


void MovementCtrl::initComponent() {
	auto* mngr = _ent->getMngr();
	_tr = mngr->getComponent<Transform>(_ent);
	assert(_tr != nullptr);
}
//como comentaba en el handleEvent del player cpp o muevo la speed de aqui a alli o
//o hago un segundo metodo Move que no aplique speed para el knockback pero... creo que mejor lo otro
void MovementCtrl::Move(Transform& transform, Vector2D movementVector, float speed) {
	transform.getVel() = movementVector * speed;
}