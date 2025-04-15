#pragma once

#include <SDL.h>
#include "GameState.h"
#include "../sdlutils/Texture.h"
#include "../sdlutils/VirtualTimer.h"
#include "../../src/ecs/Entity.h"
#include "../../src/ecs/Manager.h"
#include "../Class/Game.h"
#include "../map/RoomStorage.h"
#include "../map/DungeonFloor.h"
#include "../Class/BulletUtils.h"
#include "../Class/TimerCountdown.h"
#include "../Class/Minigame.h"

#include <chrono>

using namespace ecs;

class RunningState : virtual public GameState {
public:
	RunningState(/*Manager* mgr*/);
	~RunningState();
	bool GMG(bool minigameActive);
	BulletUtils* getBulletUtils() {return bullet;}
	void update() override;
private:
	void checkCollisions();
	//Manager* _mngr;
	BulletUtils* bullet;
	VirtualTimer asteroidSpawnTimer;
	const int asteroidSpawnCDms = 5000;

	RoomStorage* roomstorage;
	DungeonFloor* dungeonfloor;

	// Heredado via GameState
	void enter() override;
	void leave() override;
	bool colission_thisframe;
	Entity* enemycolisioned;

	TimerCountdown _timer;
	float deltaTime;

	Minigame* minigame;
};