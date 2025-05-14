#include "Transform.h"
#include "../room/TileCollisionChecker.h"

void Transform::update() {
	if (_tilechecker != nullptr) {
		if (_tilechecker->getCanMoveX() && _tilechecker->getCanMoveY()) {
			_pos = _pos + _vel;
		}
		else if (_tilechecker->getCanMoveX()) {
			_pos.setX(_pos.getX() + _vel.getX());
		}
		else if (_tilechecker->getCanMoveY()) {
			_pos.setY(_pos.getY() + _vel.getY());
		}
	}
	else {
		_pos = _pos + _vel;
	}
}