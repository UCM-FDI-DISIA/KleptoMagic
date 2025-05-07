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

	// Heredado v�a GameState
	void enter() override;
	void leave() override;

	// GamePad
	std::vector<Button*> buttons;        
	int selectedButtonIndex = -1;

	void registerButtons();
	void selectNextButton();
};