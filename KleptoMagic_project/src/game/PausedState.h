#pragma once

#include <SDL.h>
#include "GameState.h"
#include "../sdlutils/Texture.h"
#include "../Class/Game.h"
#include "../Class/Button.h"

class PausedState : virtual public GameState {
public:
	PausedState();
	~PausedState();
	void enter() override;
	void update() override;
	void leave() override;
private:
	//Texture* pressAnyKey;
	float x0, y0;
	float titleWidth, titleHeight;
	Texture* background;
	Texture* title;

	Button* resumeButton;
	Texture* resumeTexture;

	Button* homeButton;
	Texture* homeTexture;
};