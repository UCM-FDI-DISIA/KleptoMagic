#pragma once

#include <SDL.h>
#include "GameState.h"
#include "../sdlutils/Texture.h"
#include "../Class/Game.h"
#include "../Class/Button.h"

class GameOverState : virtual public GameState {
public:
	GameOverState();
	~GameOverState();
	void enter() override;
	void update() override;
private:
	Texture* background;

	Button* homeButton;
	Texture* homeTexture;

	// Heredado vía GameState
	void leave() override;
};