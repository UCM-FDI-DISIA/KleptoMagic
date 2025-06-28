#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Game.h"
#include "../sdlutils/NewInputHandler.h"

//componentes :D
#include "../ecs/Manager.h"

//states
#include "../state/RunningState.h"
#include "../state/NewGameState.h"
#include "../state/NewRoundState.h"
#include "../state/PausedState.h"
#include "../state/GameOverState.h"

using namespace std;

Game::Game() : exit(false), _mngr(nullptr) {}

bool Game::init() {
	if (!SDLUtils::Init("KleptoMagic", 800, 600, "resources/config/resources.json")) {
		std::cerr << "Error inicializando SDLUtils" << std::endl;
		return false;
	}

	if (!NewInputHandler::Init()) {
		std::cerr << "Error inicializando NewInputHandler" << std::endl;
		return false;
	}

	return true;
}

bool Game::initGame() {
	_mngr = new Manager();

	if (!EnemyUtils::Init(_mngr)) {
		std::cerr << "Error initializing EnemyUtils" << std::endl;
		return false;
	}
	if (!PlayerUtils::Init(_mngr)) {
		std::cerr << "Error initializing PlayerUtils" << std::endl;
		return false;
	}
	if(!ObjectUtils::Init(_mngr)) {
		std::cerr << "Error initializing ObjectUtils" << std::endl;
		return false;
	}

	pushState(new NewGameState());

	auto ginfo = _mngr->addEntity();
	_mngr->setHandler(ecs::hdlr::GAMEINFO, ginfo);
	//_mngr->addComponent<GameCtrl>(ginfo);

	return true;
}

Game::~Game() {

	// delete all states in the stack
	while (!_stateStack.empty()) {
		delete _stateStack.top();
		_stateStack.pop();
	}

	// release SLDUtil if the instance was created correctly.
	if (SDLUtils::HasInstance())
		SDLUtils::Release();

	// release EnemyUtils if the instance was created correctly.
	if (EnemyUtils::HasInstance())
		EnemyUtils::Release();

	// release PlayerUtils if the instance was created correctly.
	if (PlayerUtils::HasInstance())
		PlayerUtils::Release();

	if (_mngr)
		delete _mngr;
}

void Game::start() {
	exit = false;
	auto& vt = sdlutils().virtualTimer();

	vt.resetTime();

	while (!exit) {
#ifdef _DEBUG
		std::cout << "Entrando en el bucle principal" << std::endl;
#endif
		Uint32 startTime = vt.regCurrTime();
		input().update(); // Update input handler

#ifdef _DEBUG
		std::cout << "Exit: " << exit << std::endl;
#endif


		if (!_stateStack.empty()) {
			_stateStack.top()->update();
		}

#ifdef _DEBUG
		std::cout << "Se ejecutó update()" << std::endl;
#endif


		Uint32 frameTime = sdlutils().currRealTime() - startTime;
		if (frameTime < 10) SDL_Delay(10 - frameTime);
	}
}

void Game::pushState(GameState* state) {
	if (!_stateStack.empty()) {
		_stateStack.top()->leave();
	}
	_stateStack.push(state);
	_stateStack.top()->enter();
}

void Game::popState() {
	if (!_stateStack.empty()) {
		_stateStack.top()->leave();
		_stateStack.pop();
	}

	if (!_stateStack.empty()) {
		_stateStack.top()->enter();
	}
}

void Game::createItems() {
}