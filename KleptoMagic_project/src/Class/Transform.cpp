#include "Transform.h"
#include "TileCollisionChecker.h"

void Transform::update() {
	if (_tilechecker != nullptr) {
		if (_tilechecker->getCanMove()) {
			_pos = _pos + _vel;
		}
	}
	else {
		_pos = _pos + _vel;
	}
}