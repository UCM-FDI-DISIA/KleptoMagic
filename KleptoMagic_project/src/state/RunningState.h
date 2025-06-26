#pragma once

#include <SDL.h>
#include "GameState.h"
#include "../sdlutils/Texture.h"
#include "../sdlutils/VirtualTimer.h"
#include "../../src/ecs/Entity.h"
#include "../../src/ecs/Manager.h"
#include "../ecs/Game.h"
#include "../map/RoomStorage.h"
#include "../map/DungeonFloor.h"
#include "../bullet/BulletUtils.h"
#include "../room/TimerCountdown.h"
#include "../room/Minigame.h"
#include "../room/ItemStorage.h"
#include <SDL_mixer.h> 

#include <chrono>

using namespace ecs;

class RunningState : virtual public GameState {
public:
	RunningState();
	~RunningState();
	bool GMG(bool minigameActive);
	BulletUtils* getBulletUtils() {return bullet;}
	void update() override;
private:
	void checkCollisions();
	BulletUtils* bullet;

	RoomStorage* roomstorage;
	DungeonFloor* dungeonfloor;

	ItemStorage* itemStorage = nullptr;

	// Heredado via GameState
	void enter() override;
	void leave() override;
	bool colission_thisframe;
	Entity* enemycolisioned;

	TimerCountdown _timer;
	float deltaTime;

	Minigame* minigame;
	std::chrono::time_point<std::chrono::steady_clock> startTimeDelta;
	float DeltaTime;

	// Tutorial
	Texture* controlsTexture;
	std::chrono::time_point<std::chrono::steady_clock> controlsTextureStartTime;

	// Corazones
	Texture* hearthTexture;
	Texture* hearthTotalTexture;
	float hp = 0;

	// Música
	Mix_Music* gameBGM; 
};