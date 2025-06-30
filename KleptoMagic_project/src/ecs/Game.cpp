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
#ifdef _DEBUG
		std::cerr << "Error inicializando SDLUtils" << std::endl;
#endif
		return false;
	}

	if (!NewInputHandler::Init()) {
#ifdef _DEBUG
		std::cerr << "Error inicializando NewInputHandler" << std::endl;
#endif
		return false;
	}

	// Inicializar SDL_mixer
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
#ifdef _DEBUG
		std::cerr << "SDL_mixer no pudo inicializarse: " << Mix_GetError() << std::endl;
#endif
		return false;
	}

	if (hurtSound == nullptr) {
		hurtSound = Mix_LoadWAV("resources/sound/hurt.mp3");
#ifdef _DEBUG
		std::cerr << "Error cargando hurt sound: " << Mix_GetError() << std::endl;
#endif
	}

	return true;
}

bool Game::initGame() {
	_mngr = new Manager();

	if (!EnemyUtils::Init(_mngr)) {
#ifdef _DEBUG
		std::cerr << "Error initializing EnemyUtils" << std::endl;
#endif
		return false;
	}
	if (!PlayerUtils::Init(_mngr)) {
#ifdef _DEBUG
		std::cerr << "Error initializing PlayerUtils" << std::endl;
#endif
		return false;
	}
	if(!ObjectUtils::Init(_mngr)) {
#ifdef _DEBUG
		std::cerr << "Error initializing ObjectUtils" << std::endl;
#endif
		return false;
	}

	pushState(new NewGameState());

	auto ginfo = _mngr->addEntity();
	_mngr->setHandler(ecs::hdlr::GAMEINFO, ginfo);
	//_mngr->addComponent<GameCtrl>(ginfo);

	return true;
}

Game::~Game() {

	// Eliminar estados
	while (!_stateStack.empty()) {
		delete _stateStack.top();
		_stateStack.pop();
	}

	// Liberar sonido
	if (hurtSound != nullptr) {
		Mix_FreeChunk(hurtSound);
		hurtSound = nullptr;
	}

	// Cerrar SDL_mixer completamente
	Mix_CloseAudio();
	Mix_Quit(); 

	// Liberar Manager
	if (_mngr) {
		delete _mngr;
		_mngr = nullptr;
	}

	// Liberar ObjectUtils
	if (ObjectUtils::HasInstance())
		ObjectUtils::Release();

	// Liberar EnemyUtils
	if (EnemyUtils::HasInstance())
		EnemyUtils::Release();

	// Liberar PlayerUtils
	if (PlayerUtils::HasInstance())
		PlayerUtils::Release();

	// Liberar InputHandler
	if (NewInputHandler::HasInstance())
		NewInputHandler::Release();

	// Liberar SDLUtils
	if (SDLUtils::HasInstance())
		SDLUtils::Release();
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