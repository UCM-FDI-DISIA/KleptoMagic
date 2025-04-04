#pragma once
#include <SDL.h>
#include "../utils/Vector2D.h"
#include "../ecs/Component.h"
#include "Transform.h"
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
#include "../map/DungeonFloor.h"

enum TileCollision {
	COL_FLOOR = 0,
	COL_WALL = 1,
	COL_HOLE = 2
};

class TileCollisionChecker : public ecs::Component {
public:
	__CMPID_DECL__(ecs::cmp::TILECOLLISIONCHECKER);

	void initComponent() override;
	void init(Transform* tr, DungeonFloor* floor);
	void update();
	TileCollision getCurrCollision() { return currentCollision; };

private:
	void createStart();
	Transform* _tr;
	DungeonFloor* dungeonfloor;
	// Tells you the current type of tile you are about to collide (stand) on the next frame. It accounts for the center of the entity. 
	// 0 = Floor. 1 = Wall. 2 = Hole.
	TileCollision currentCollision;
};