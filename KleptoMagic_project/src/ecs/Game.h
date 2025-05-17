#ifndef GAME_H
#define GAME_H

#include <array>
#include <vector>
#include <SDL.h>
#include <stack>
#include "../sdlutils/Texture.h"

//componentes :D
#include "../utils/Singleton.h"
#include "../sdlutils/SDLUtils.h"
#include "../enemies/EnemyUtils.h"
#include "../state/GameState.h"
#include "../player/PlayerUtils.h"
#include"../ecs/EventHandler.h"


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

public:
	bool init();
	bool initGame();
	void start();
	virtual ~Game();

	inline ecs::Manager* getMngr() { return _mngr; }

	//getter
	//InputManager* getInputManager() { return _inputManager; }
	SDL_Renderer* getRenderer() const { return renderer; }

	void setSelectedCharacter(std::string character) {
		selectedCharacter = character;
		Characters c = CHAR_KNIGHT; // default
		if (character == "KNIGHT") c = CHAR_KNIGHT;
		else if (character == "HUNTER") c = CHAR_HUNTER;
		else if (character == "ROGUE") c = CHAR_ROGUE;
		else if (character == "ALCHEMIST") c = CHAR_ALCHEMIST;
		playerutils().selectCharacter(c);
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
	void pushState(GameState* state);

	void setEndResult(bool result) 
	{
		endResult = result;
	}
	bool getResult() 
	{
		return endResult;
	}

private:
	Game();
	ecs::Manager* _mngr;

	// Interruptor para terminar el juego
	bool exit;

	std::stack<GameState*> _stateStack;

	void createItems();
	// Para menu de seleccion de personajes
	std::string selectedCharacter;

	bool endResult;
};

inline Game& game() {
	return *Game::Instance();
}
#endif