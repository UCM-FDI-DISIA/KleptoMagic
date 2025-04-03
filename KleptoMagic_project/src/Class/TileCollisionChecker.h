#pragma once
#include <SDL.h>
#include "../utils/Vector2D.h"
#include "../ecs/Component.h"
#include "Transform.h"
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
#include "../map/DungeonFloor.h"

enum TileCollision {
	FLOOR = 0,
	WALL = 1,
	HOLE = 2
};

class TileCollisionChecker : public ecs::Component {
public:
	__CMPID_DECL__(ecs::cmp::TILECOLLISIONCHECKER);

	void initComponent() override;
	void setDungeonFloor(DungeonFloor* floor);
	void update();

private:
	void createStart();
	Transform* _tr;
	DungeonFloor* dungeonfloor;
	TileCollision currentCollision;
};