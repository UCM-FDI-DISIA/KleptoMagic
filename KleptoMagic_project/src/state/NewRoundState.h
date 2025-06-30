#pragma once

#include <SDL.h>
#include "GameState.h"
#include "../sdlutils/Texture.h"
#include "../ecs/Game.h"
#include "../ecs/Button.h"

class NewRoundState : virtual public GameState {
public:
	NewRoundState();
	~NewRoundState();
	void update() override;
private:
	Texture* background;
	Texture* rogue;
	Texture* knight;
	Texture* alchemist;
	Texture* hunter;
	// Texturas de botones
	Texture* rogueButtonTexture;
	Texture* knightButtonTexture;
	Texture* alchemistButtonTexture;
	Texture* hunterButtonTexture;

	Button* rogueButton;
	Button* knightButton;
	Button* alchemistButton;
	Button* hunterButton;

	std::string selectedCharacter;

	float imgSize = 100;
	float btnWidth = 140; // Ancho del boton
	float btnHeight = 100; // Alto del boton
	float spacing = 12; // Espacio entre botones

	// Heredado vía GameState
	void enter() override;
	void leave() override;
};