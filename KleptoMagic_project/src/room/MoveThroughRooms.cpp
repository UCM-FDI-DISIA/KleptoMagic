#include "MoveThroughRooms.h"
#include "../enemies/EnemyUtils.h"
#include "../bullet/BulletUtils.h"
void MoveThroughRooms::initComponent() { //falta get entity stats speed
	auto* _mngr = _ent->getMngr();
	_tr = _mngr->getComponent<Transform>(_ent);
	assert(_tr != nullptr);
	
}

void MoveThroughRooms::init(DungeonFloor* floor, BulletUtils* bull) {
	_dungeonfloor = floor;
	bullet = bull;
}

void MoveThroughRooms::enterRoom(char exit) {
	Vector2D position = _dungeonfloor->enterRoom(exit);
	float position_realX = position.getX() - (_tr->getWidth() / 2);
	float position_realY = position.getY() - (_tr->getHeight() / 2);
	_tr->getPos() = Vector2D{position_realX, position_realY};
	enemyutils().remove_all_enemies();
	bullet->reset();
	if (!_dungeonfloor->getCurrentRoom()->isCleared()) {
		_dungeonfloor->spawnEnemies();
	}
}

void MoveThroughRooms::update() {
	auto pos = _tr->getPos();
	int centerX = pos.getX() + (_tr->getWidth() / 2);
	int centerY = pos.getY() + (_tr->getHeight() - _tr->getHeight() / 16);

	int xPostMove = centerX + _tr->getVel().getX();
	int yPostMove = centerY + _tr->getVel().getY();

	char result = _dungeonfloor->checkEnterExit(xPostMove, yPostMove);

	if ((result == 'U' || result == 'D' || result == 'L' || result == 'R')
		&& _dungeonfloor->isRoomCleared()) {
		cout << _dungeonfloor->isRoomCleared() << endl;
		enterRoom(result);
	}
}