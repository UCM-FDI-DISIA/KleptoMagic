#ifndef GAME_H
#define GAME_H

#include <array>
#include <vector>
#include <SDL.h>
//#include "checkML.h"
#include "../sdlutils/Texture.h"
//#include "GameObject.h"
//#include "SceneObject.h"
//#include "TileMap.h"
//#include "Player.h"
//#include "gameList.h"
//#include "PlayState.h"
//#include "MainMenuState.h"
//#include "PauseState.h"
//#include "AnimationState.h"
//#include "EndState.h"
#include "gameStateMachine.h"
class DummyState;
class MainMenuState;
class PlayState;

#pragma once

using uint = unsigned int;

class TileMap;

class Game : public GameStateMachine
{
public:
	// Identificadores de las texturas
	enum TextureName {
		MAINMENUBACKGROUND,
		MAINMENUBUTTON,
		ENDMENUBACKGROUND,
		ENDMENUBUTTON,
		PLAYER,
		NUM_TEXTURES
	};
	Texture* getTexture(TextureName name) const;

	// Constante globales
	static constexpr uint WIN_WIDTH = 544;
	static constexpr uint WIN_HEIGHT = 480;
	static constexpr uint FRAME_RATE = 60;
	static constexpr uint TILE_SIZE = 32;

	int getWindowWidth() const { return WIN_WIDTH; }
	int getWindowHeight() const { return WIN_HEIGHT; }

	// Ventana de la SDL (se destruir� en el destructor)
	SDL_Window* window = nullptr;
	// Renderizador de la SDL (para dibujar)
	SDL_Renderer* renderer = nullptr;
	SDL_Renderer* getRenderer() const { return renderer; }

	// Gamestates
	PlayState* playstate;
	MainMenuState* mainmenu;
	//PauseState* pausestate;
	//AnimationState* animationstate;
	//EndState* endstate;
	DummyState* dummy;

	Game();
	//~Game();
	void run();
	void statePlay();
	void statePause();
	void stateNotPause();
	void stateMainMenu();
	//void stateAnimation(function<bool()> funcAnim);
	void stateAnimationEnd();
	void stateEnd();
	void gameExit();
private:
	// Array con todas las texturas del juego
	std::array<Texture*, NUM_TEXTURES> textures;
	// Interruptor para terminar el juego
	bool exit;

};

#endif