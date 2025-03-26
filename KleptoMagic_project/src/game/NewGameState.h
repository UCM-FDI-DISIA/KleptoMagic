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
	//Texture* pressAnyKey;
	//float x0, y0;
	Texture* background;  
	Texture* buttonTexture;  
	Button* startButton;  

	// Heredado v�a GameState
	void enter() override;
	void leave() override;
};