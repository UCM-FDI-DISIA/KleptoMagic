#include "MoveThroughRooms.h"
#include "../game/EnemyUtils.h"

void MoveThroughRooms::initComponent() { //falta get entity stats speed
	auto* _mngr = _ent->getMngr();
	_tr = _mngr->getComponent<Transform>(_ent);
	assert(_tr != nullptr);
}

void MoveThroughRooms::init(DungeonFloor* floor) {
	_dungeonfloor = floor;
}

void MoveThroughRooms::enterRoom(char exit) {
	Vector2D position = _dungeonfloor->enterRoom(exit);
	float position_realX = position.getX() - (_tr->getWidth() / 2);
	float position_realY = position.getY() - (_tr->getHeight() / 2);
	_tr->getPos() = Vector2D{position_realX, position_realY};
	enemyutils().remove_all_enemies();
}

void MoveThroughRooms::update() {
	auto pos = _tr->getPos();
	int centerX = pos.getX() + (_tr->getWidth() / 2);
	int centerY = pos.getY() + (_tr->getHeight() / 2);

	int xPostMove = centerX + _tr->getVel().getX();
	int yPostMove = centerY + _tr->getVel().getY();

	char result = _dungeonfloor->checkEnterExit(xPostMove, yPostMove);

	if (result != ' ') {
		enterRoom(result);
	}
}