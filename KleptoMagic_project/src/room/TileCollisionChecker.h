#pragma once
#include <SDL.h>
#include "../utils/Vector2D.h"
#include "../ecs/Component.h"
#include "../ecs/Transform.h"
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
	// Flying = true means the entity will not treat holes as walls.
	void init(bool flying, Transform* tr, DungeonFloor* floor);
	void update();
	TileCollision getCurrCollisionType() { return currentCollision; };
	bool getCanMove() { return canMove; };
	bool getCanMoveX() { return canMoveX; };
	bool getCanMoveY() { return canMoveY; };

private:
	void createStart();
	Transform* _tr;
	DungeonFloor* dungeonfloor;
	// Tells you the current type of tile you are about to collide (stand) on the next frame. It accounts for the center of the entity. 
	// 0 = Floor. 1 = Wall. 2 = Hole.
	TileCollision currentCollision;
	// Dictates if the entity considers holes to be collidable or not
	bool canFly;
	// Shows whether or not the entity can move according to the position it'll be in next frame
	bool canMove, canMoveX, canMoveY;
};