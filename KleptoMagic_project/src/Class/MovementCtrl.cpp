#include "MovementCtrl.h"


void MovementCtrl::Move(Vector2D& pos, Vector2D& movement, float speed) {
	pos = pos + movement * speed;
	movement = movement * 0;
}