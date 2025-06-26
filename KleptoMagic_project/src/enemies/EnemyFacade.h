#pragma once

#include <SDL.h>
#include "../sdlutils/SDLUtils.h"
#include "../../../src/ecs/Entity.h"
#include "../../../src/ecs/Manager.h"

class DungeonFloor;

enum EnemyNames {
	ENEMY_SLIME,
	ENEMY_ARCHER,
	ENEMY_ARMOR,
	ENEMY_GHOST,
	ENEMY_BOSS,
	ENEMY_NECRO,
	ENEMY_CHEST,
};

using namespace ecs;

class EnemyFacade {
public:
	EnemyFacade() {}
	virtual ~EnemyFacade() {}
	virtual void setDungeonFloor(DungeonFloor* floor) = 0;
	virtual void spawn_enemy(EnemyNames name, Vector2D pos) = 0;
	virtual void necro_spawn(Entity* necro,int x, int y) = 0;
	virtual void remove_all_enemies() = 0;
};
