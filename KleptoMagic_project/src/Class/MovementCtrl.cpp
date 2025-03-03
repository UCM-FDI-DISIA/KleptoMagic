#include "MovementCtrl.h"

//como comentaba en el handleEvent del player cpp o muevo la speed de aqui a alli o
//o hago un segundo metodo Move que no aplique speed para el knockback pero... creo que mejor lo otro
void MovementCtrl::Move(Vector2D& pos, Vector2D& movement, float speed) {
	pos = pos + movement * speed;
	movement = movement * 0;
}