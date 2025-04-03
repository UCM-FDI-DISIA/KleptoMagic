#include "TileCollisionChecker.h"
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
#include "../sdlutils/SDLUtils.h"


void TileCollisionChecker::initComponent() {
	auto* _mngr = _ent->getMngr();
	_tr = _mngr->getComponent<Transform>(_ent);
	assert(_tr != nullptr);
}

void TileCollisionChecker::setDungeonFloor(DungeonFloor* floor) {
	dungeonfloor = floor;
}

void TileCollisionChecker::update() {
	auto pos = _tr->getPos();
	switch (dungeonfloor->checkCollisions(pos.getX(), pos.getY())) {
	case 0:
		currentCollision = FLOOR;
		break;
	case 1:
		currentCollision = WALL;
		break;
	case 2:
		currentCollision = HOLE;
		break;
	default:
		currentCollision = FLOOR;
		break;
	}
}