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

public:
	
	void spawnPlayer(Vector2D pos);
};

// This macro defines a compact way for using the singleton
inline PlayerUtils& enemyutils() {
	return *PlayerUtils::Instance();
}
