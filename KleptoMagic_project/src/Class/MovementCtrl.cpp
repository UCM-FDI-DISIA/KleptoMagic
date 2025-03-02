#include "MovementCtrl.h"


void MovementCtrl::Move(Vector2D& pos, Vector2D& speed) {
	pos = pos + speed;
	speed = speed * 0;
}