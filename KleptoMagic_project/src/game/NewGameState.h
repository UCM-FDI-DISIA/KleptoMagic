#pragma once

#include <SDL.h>
#include "GameState.h"
#include "../sdlutils/Texture.h"
#include "../Class/Game.h"
#include "../Class/Button.h"

class NewGameState : virtual public GameState {
public:
	NewGameState();
	~NewGameState();
	void update() override;
private:
	Texture* background;  
	Texture* buttonTexture;  
	Texture* exitButtonTexture;
	Button* startButton;  
	Button* exitButton;
	Uint32 releaseTime = 0;  // Nueva variable para controlar la espera

	// Heredado vía GameState
	void enter() override;
	void leave() override;
};