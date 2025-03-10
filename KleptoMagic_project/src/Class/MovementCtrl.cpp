#include "MovementCtrl.h"
//#include <iostream>


void MovementCtrl::initComponent() {
	//auto* mngr = _ent->getMngr();
	//_tr = mngr->getComponent<Transform>(_ent);
	//assert(_tr != nullptr);
}
//como comentaba en el handleEvent del player cpp o muevo la speed de aqui a alli o
//o hago un segundo metodo Move que no aplique speed para el knockback pero... creo que mejor lo otro
void MovementCtrl::Move(Vector2D& pos, Vector2D& movement, float speed) {
	//std::cout << movement.magnitude() << std::endl;
	pos = pos + movement * speed;
	movement = movement * 0;
}