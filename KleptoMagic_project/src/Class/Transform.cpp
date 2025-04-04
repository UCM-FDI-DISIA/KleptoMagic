#include "Transform.h"
#include "TileCollisionChecker.h"

void Transform::update() {
	if (_tilechecker != nullptr) {

		auto collision = _tilechecker->getCurrCollision();
		cout << collision << endl;
		if (collision == 0) {
			_pos = _pos + _vel;
		}
	}
	else {
		_pos = _pos + _vel;
	}
}