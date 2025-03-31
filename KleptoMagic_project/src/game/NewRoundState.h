#pragma once

#include <SDL.h>
#include "GameState.h"
#include "../sdlutils/Texture.h"
#include "../Class/Game.h"
#include "../Class/Button.h"

class NewRoundState : virtual public GameState {
public:
	NewRoundState();
	~NewRoundState();
	void update() override;
private:
	Texture* pressEnter;
	float x0, y0;
	Texture* background;
	//Texture* buttonTexture;
	//Button* selectButton;
	Button* rogueButton;
	Button* knightButton;
	Button* alchemistButton;
	Button* hunterButton;

	std::string selectedCharacter;

	// Heredado vía GameState
	void enter() override;
	void leave() override;
};