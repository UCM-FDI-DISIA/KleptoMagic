#pragma once

#include <SDL.h>
#include "GameState.h"
#include "../sdlutils/Texture.h"
#include "../sdlutils/VirtualTimer.h"
#include "../../../src/ecs/Entity.h"
#include "../../../src/ecs/Manager.h"
#include "../Class/Game.h"

using namespace ecs;

class RunningState : virtual public GameState {
public:
	RunningState(Manager* mgr);
	~RunningState();
	void update() override;
private:
	void checkCollisions();
	Manager* _mngr;
	SDL_Renderer* renderer;
	//VirtualTimer asteroidSpawnTimer;
	//const int asteroidSpawnCDms = 5000;

	// Heredado vía GameState
	void enter() override;
	void leave() override;
	bool colission_thisframe;
};