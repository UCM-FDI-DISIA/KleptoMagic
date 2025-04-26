#pragma once

#include "PlayerFacade.h"
#include "../../src/utils/Singleton.h"
#include "../utils/Vector2D.h"


using namespace ecs;

class PlayerUtils : virtual public PlayerFacade, public Singleton<PlayerUtils> {
	friend Singleton<PlayerUtils>;
private:
	PlayerUtils();
	bool init(Manager* mngr);
	virtual ~PlayerUtils();
	Manager* _mngr;
	DungeonFloor* _dungeonfloor;
	Characters _character;

public:
	void selectCharacter(Characters c) { _character = c; };
	void createPlayer(Vector2D pos, float size);
	void destroyPlayer();
};

// This macro defines a compact way for using the singleton
inline PlayerUtils& playerutils() {
	return *PlayerUtils::Instance();
}
