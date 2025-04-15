#pragma once

#include <SDL.h>
#include "GameState.h"
#include "../sdlutils/Texture.h"
#include "../Class/Game.h"

class GameOverState : virtual public GameState {
public:
	GameOverState();
	~GameOverState();
	void enter() override;
	void update() override;
private:
	Texture* background;

	// Heredado vía GameState
	void leave() override;
};