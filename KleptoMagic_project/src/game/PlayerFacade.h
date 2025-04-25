#pragma once

#include <SDL.h>
#include "../sdlutils/SDLUtils.h"
#include "../../../src/ecs/Entity.h"
#include "../../../src/ecs/Manager.h"

class DungeonFloor;

enum Characters {
	KNIGHT = 0,
	HUNTER = 1,
	ROGUE = 2,
	ALCHEMIST = 3,
};

using namespace ecs;

class PlayerFacade {
public:
	PlayerFacade() {}
	virtual ~PlayerFacade() {}
	virtual void spawnPlayer() = 0;
	virtual void damagePlayer() = 0;
	virtual void resetPlayer() = 0;
};