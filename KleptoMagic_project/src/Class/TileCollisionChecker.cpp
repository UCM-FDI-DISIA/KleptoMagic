#include "TileCollisionChecker.h"
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
#include "../sdlutils/SDLUtils.h"
#include <iostream>

void TileCollisionChecker::initComponent() {
	auto* _mngr = _ent->getMngr();
}

void TileCollisionChecker::init(Transform* tr, DungeonFloor* floor) {
	_tr = tr;
	dungeonfloor = floor;
}

void TileCollisionChecker::update() {
	auto pos = _tr->getPos();
	int centerX = pos.getX() + (_tr->getWidth() / 2);
	int centerY = pos.getY() + (_tr->getHeight() / 2);

	int xPostMove = centerX + _tr->getVel().getX();
	int yPostMove = centerY + _tr->getVel().getY();

	int result = dungeonfloor->checkCollisions(xPostMove, yPostMove);
	switch (result) {
	case 0:
		currentCollision = COL_FLOOR;
		break;
	case 1:
		currentCollision = COL_WALL;
		break;
	case 2:
		currentCollision = COL_HOLE;
		break;
	default:
		currentCollision = COL_FLOOR;
		break;
	}
}