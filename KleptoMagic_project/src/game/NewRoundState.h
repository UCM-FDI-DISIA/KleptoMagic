#pragma once

#include <SDL.h>
#include "GameState.h"
#include "../sdlutils/Texture.h"
#include "../Class/Game.h"

class NewRoundState : virtual public GameState {
public:
	NewRoundState();
	~NewRoundState();
	void update() override;
private:
	Texture* pressEnter;
	float x0, y0;

	// Heredado vía GameState
	void enter() override;
	void leave() override;
};