#ifndef GAME_H
#define GAME_H

#include <array>
#include <vector>
#include <SDL.h>
#include <stack>
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

#include "../game/EnemyUtils.h"


#pragma once

using uint = unsigned int;

class TileMap;

namespace ecs {
	class Manager;
}

class Game : public Singleton<Game>
{
	friend Singleton<Game>;

protected:
	// Constante globales
	static constexpr uint WIN_WIDTH = 544;
	static constexpr uint WIN_HEIGHT = 480;
	static constexpr uint FRAME_RATE = 60;
	static constexpr uint TILE_SIZE = 32;

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

	//getter
	//InputManager* getInputManager() { return _inputManager; }
	SDL_Renderer* getRenderer() const { return renderer; }

	void setSelectedCharacter(std::string character) {
#ifdef _DEBUG
		std::cout << "Guardando personaje: " << character << std::endl;
#endif
		selectedCharacter = character;
	}

	std::string getSelectedCharacter() {
#ifdef _DEBUG
		std::cout << "Recuperando personaje: " << selectedCharacter << std::endl;
#endif
		return selectedCharacter;
	}

	int getWindowWidth() const { return WIN_WIDTH; }
	int getWindowHeight() const { return WIN_HEIGHT; }

	inline void exitGame() {
		exit = true;
	}

	void popState();
	void setGameState(GameState* state);
	void pushState(GameState* state);

private:
	Game();
	ecs::Manager* _mngr;

	// Interruptor para terminar el juego
	bool exit;

	std::stack<GameState*> _stateStack;

	void createItems();
	// Para menu de seleccion de personajes
	std::string selectedCharacter;
};

inline Game& game() {
	return *Game::Instance();
}
#endif