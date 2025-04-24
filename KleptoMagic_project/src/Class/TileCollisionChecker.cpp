#include "TileCollisionChecker.h"
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
#include "../sdlutils/SDLUtils.h"
#include <iostream>

void TileCollisionChecker::initComponent() {
	auto* _mngr = _ent->getMngr();
}

void TileCollisionChecker::init(bool flying, Transform* tr, DungeonFloor* floor) {
	_tr = tr;
	dungeonfloor = floor;
	canFly = flying;
	canMove = true;
}

void TileCollisionChecker::update() {
	auto pos = _tr->getPos();
	int centerX = pos.getX() + (_tr->getWidth() / 2);
	int centerY = pos.getY() + (_tr->getHeight() / 2);

	Vector2D vel = _tr->getVel();

	// Check X-axis movement
	int xPostMove = centerX + vel.getX();
	int yFixed = centerY;
	int resultX = dungeonfloor->checkCollisions(xPostMove, yFixed);

	switch (resultX) {
	case 0:
		canMoveX = true;
		currentCollision = COL_FLOOR;
		break;
	case 2:
		canMoveX = canFly;
		currentCollision = COL_HOLE;
		break;
	default:
		canMoveX = false;
		currentCollision = COL_WALL;
		break;
	}

	// Check Y axis movement
	int xFixed = centerX;
	int yPostMove = centerY + vel.getY();
	int resultY = dungeonfloor->checkCollisions(xFixed, yPostMove);

	switch (resultY) {
	case 0:
		canMoveY = true;
		currentCollision = COL_FLOOR;
		break;
	case 2:
		canMoveY = canFly;
		currentCollision = COL_HOLE;
		break;
	default:
		canMoveY = false;
		currentCollision = COL_WALL;
		break;
	}

	// Check if the entity can move at least in one direction
	canMove = canMoveX || canMoveY;

	std::cout << "Current collision: " << currentCollision << std::endl;
}
