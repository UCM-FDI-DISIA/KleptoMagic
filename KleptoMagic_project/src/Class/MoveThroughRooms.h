#pragma once
#include <SDL.h>
#include "../utils/Vector2D.h"
#include "../ecs/Component.h"
#include "Transform.h"
#include "../map/DungeonFloor.h"
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
class BulletUtils;
class MoveThroughRooms : public ecs::Component
{
public:
	__CMPID_DECL__(ecs::cmp::MOVETHROUGHROOMS);

	void initComponent() override;
	void init(DungeonFloor* floor, BulletUtils* bull);
	void enterRoom(char exit);
	void update();

private:
	void createStart();
	Transform* _tr;
	BulletUtils* bullet;
	DungeonFloor* _dungeonfloor;
};