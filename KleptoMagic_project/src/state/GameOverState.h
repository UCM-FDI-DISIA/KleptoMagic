#pragma once

#include <SDL.h>
#include "GameState.h"
#include "../sdlutils/Texture.h"
#include "../ecs/Game.h"
#include "../ecs/Button.h"
#include <SDL_mixer.h> 

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

	Texture* winText;
	Texture* loseText;

	// Heredado v�a GameState
	void leave() override;

	// Sonidos
	Mix_Chunk* winSound;
	Mix_Chunk* loseSound;
	bool soundPlayed;
};