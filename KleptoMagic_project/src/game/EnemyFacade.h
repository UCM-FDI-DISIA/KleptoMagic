#pragma once

#include <SDL.h>
#include "../sdlutils/SDLUtils.h"
#include "../../../src/ecs/Entity.h"
#include "../../../src/ecs/Manager.h"

class DungeonFloor;

enum EnemyNames {
	ENEMY_SLIME = 0,
	ENEMY_ARCHER = 1,
	ENEMY_ARMOR = 2, 
	ENEMY_BOSS = 3
};

using namespace ecs;

class EnemyFacade {
public:
	EnemyFacade() {}
	virtual ~EnemyFacade() {}
	virtual void setDungeonFloor(DungeonFloor* floor) = 0;
	virtual void spawn_enemy(EnemyNames name, Vector2D pos) = 0;
	virtual void remove_all_enemies() = 0;
};
