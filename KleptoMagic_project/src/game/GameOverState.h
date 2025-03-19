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
	Texture* text;
	float x0, y0;

	// Heredado vía GameState
	void leave() override;
};