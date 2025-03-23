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
//#include "gameStateMachine.h"
//class DummyState;
//class MainMenuState;
//class PlayState;

//componentes :D
#include "../utils/Singleton.h"
#include "../sdlutils/SDLUtils.h"
#include "../game/GameState.h"

#pragma once

using uint = unsigned int;

class TileMap;

namespace ecs {
	class Manager;
}

class Game : public Singleton<Game>
{
	friend Singleton<Game>;

public:
	// Identificadores de las texturas
	enum TextureName {
		ENDMENUBACKGROUND,
		ENDMENUBUTTON,
		PLAYER,
		NUM_TEXTURES
	};
	//Texture* getTexture(TextureName name) const;

	enum State {
		RUNNING, PAUSED, NEWGAME, NEWROUND, GAMEOVER
	};

protected:
	// Constante globales
	static constexpr uint WIN_WIDTH = 544;
	static constexpr uint WIN_HEIGHT = 480;
	static constexpr uint FRAME_RATE = 60;
	static constexpr uint TILE_SIZE = 32;
public:
	int getWindowWidth() const { return WIN_WIDTH; }
	int getWindowHeight() const { return WIN_HEIGHT; }
protected:
	// Ventana de la SDL (se destruir� en el destructor)
	SDL_Window* window = nullptr;
	// Renderizador de la SDL (para dibujar)
	SDL_Renderer* renderer = nullptr;

	// Gamestates
	//PlayState* playstate;
	//MainMenuState* mainmenu;
	//PauseState* pausestate;
	//AnimationState* animationstate;
	//EndState* endstate;
	//DummyState* dummy;

	//InputManager* _inputManager;

public:
	bool init();
	bool initGame();
	void start();
	virtual ~Game();

	//void run();

	//~Game();
	//void statePlay();
	//void statePause();
	//void stateNotPause();
	//void stateMainMenu();
	////void stateAnimation(function<bool()> funcAnim);
	//void stateAnimationEnd();
	//void stateEnd();
	//void gameExit();

	inline ecs::Manager* getMngr() { return _mngr; }
	inline void setState(State s) {
		_state->leave();
		sdlutils().clearRenderer();
		switch (s) {
		case RUNNING:
			_state = _running_state;
			break;
		case PAUSED:
			_state = _paused_state;
			break;
		case NEWGAME:
			_state = _newgame_state;
			break;
		case NEWROUND:
			_state = _newround_state;
			break;
		case GAMEOVER:
			_state = _gameover_state;
			break;
		default:
			break;
		}
		_state->enter();
	};

	//getter
	//InputManager* getInputManager() { return _inputManager; }
	SDL_Renderer* getRenderer() const { return renderer; }

private:
	Game();
	ecs::Manager* _mngr;

	GameState* _state;
	GameState* _paused_state;
	GameState* _running_state;
	GameState* _newgame_state;
	GameState* _newround_state;
	GameState* _gameover_state;
	// Array con todas las texturas del juego
	std::array<Texture*, NUM_TEXTURES> textures;
	// Interruptor para terminar el juego
	bool exit;

};

inline Game& game() {
	return *Game::Instance();
}
#endif