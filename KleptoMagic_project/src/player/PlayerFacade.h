#pragma once

#include <SDL.h>
#include "../sdlutils/SDLUtils.h"
#include "../../../src/ecs/Entity.h"
#include "../../../src/ecs/Manager.h"

class DungeonFloor;
class BulletUtils;

enum Characters {
	CHAR_KNIGHT = 0,
	CHAR_HUNTER = 1,
	CHAR_ROGUE = 2,
	CHAR_ALCHEMIST = 3,
};

using namespace ecs;

class PlayerFacade {
public:
	PlayerFacade() {}
	virtual ~PlayerFacade() {}
	virtual void setDungeonFloor(DungeonFloor* floor) = 0;
	virtual void selectCharacter(Characters c) = 0;
	virtual void createPlayer(Vector2D pos, float size, BulletUtils* bullet) = 0;
	virtual void destroyPlayer() = 0;
};