#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "gameStateMachine.h"
#include "Game.h"
#include "DummyState.h"
#include "MainMenuState.h"
#include "PlayState.h"
#include "../sdlutils/NewInputHandler.h"

//componentes :D
#include "../ecs/Manager.h"

//states
#include "../game/RunningState.h"
#include "../game/NewGameState.h"
#include "../game/NewRoundState.h"
#include "../game/PausedState.h"
#include "../game/GameOverState.h"

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

	setGameState(new NewGameState());

	auto ginfo = _mngr->addEntity();
	_mngr->setHandler(ecs::hdlr::GAMEINFO, ginfo);
	//_mngr->addComponent<GameCtrl>(ginfo);

	return true;
}

Game::~Game() {
	delete _mngr;

	// release SLDUtil if the instance was created correctly.
	if (SDLUtils::HasInstance())
		SDLUtils::Release();

	// release AsteroidsUtils if the instance was created correctly.
	if (EnemyUtils::HasInstance())
		EnemyUtils::Release();
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

		NewInputHandler::Instance()->update();
		//ihdlr.refresh();

		if (NewInputHandler::Instance()->isActionPressed(Action::SHOOT)) {
			exit = true;
			continue;
		}
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

void Game::setGameState(GameState* state) {
	if (!_stateStack.empty()) {
		_stateStack.top()->leave();
	}
	_stateStack.push(state);
	_stateStack.top()->enter();
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

//void
//Game::run()
//{
//	//delete prevWorld;
//
//	// Bucle principal del juego
//
//	while (!exit) {
//		// Marca de tiempo del inicio de la iteración
//		uint32_t inicio = SDL_GetTicks();
//
//		//SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
//
//		SDL_Event evento;
//		GameStateMachine::handleEvent(evento);
//		while (SDL_PollEvent(&evento)) {
//			if (evento.type == SDL_QUIT)
//				exit = true;
//			else {
//				GameStateMachine::handleEvent(evento);
//			}
//		}
//
//		_inputManager->update();
//
//
//		GameStateMachine::update();
//		SDL_RenderClear(renderer);
//		GameStateMachine::render();
//		SDL_RenderPresent(renderer);
//
//		
//		
//		//playstate->update();       // Actualiza el estado de los objetos del juego
//		//playstate->render();       // Dibuja los objetos en la venta
//		//playstate->handleEvents(); // Maneja los eventos de la SDL (por defecto estaba después de render?)
//
//		// Tiempo que se ha tardado en ejecutar lo anterior
//		uint32_t elapsed = SDL_GetTicks() - inicio;
//
//		// Duerme el resto de la duraci󮠤el frame
//		if (elapsed < Game::FRAME_RATE)
//			SDL_Delay(Game::FRAME_RATE - elapsed);
//	}
//}
//
///*Texture* Game::getTexture(TextureName name) const {
//	return textures[name];  // Return the texture based on the enum index
//}*/
//Texture* Game::getTexture(TextureName name) const {
//	if (textures[name] == nullptr) {
//#ifdef _DEBUG
//		std::cerr << "Error: La textura " << name << " no está cargada." << std::endl;
//#endif
//	}
//	return textures[name];
//}
//
////void Game::statePlay(int w) {
////#ifdef _DEBUG
////	std::cout << "Cambiando a PlayState..." << std::endl;
////#endif
////	GameStateMachine::popState(); // Eliminar MainMenuState
////	auto playstate = new PlayState(); // Crear PlayState
////	GameStateMachine::pushState(playstate); // Ponerlo en la pila
////}


//void Game::statePlay() {
//	playstate = new PlayState(this);
//	GameStateMachine::replaceState(playstate);
//}
//
////void Game::statePause(){
////	pausestate = new PauseState(this);
////	GameStateMachine::pushState(pausestate);
////}
////
////void Game::stateNotPause(){
////	GameStateMachine::popState();
////}
////
//void Game::stateMainMenu() {
//	GameStateMachine::popState();
//	mainmenu = new MainMenuState(this, textures[Game::BACKGROUND]); //se puede comentar y ver si va
//	GameStateMachine::pushState(mainmenu);
//}
//
/*void Game::stateMainMenu() {
	GameStateMachine::popState();
	mainmenu = new MainMenuState(this, textures[Game::MAINMENUBACKGROUND]); //se puede comentar y ver si va
	GameStateMachine::pushState(mainmenu);
}*/

//void Game::stateAnimation(function<bool()> funcAnim) {
//	animationstate = new AnimationState(this, playstate, funcAnim);
//	GameStateMachine::pushState(animationstate);
//}
//
//void Game::stateAnimationEnd() {
//	GameStateMachine::popState();
//	playstate->map_reload();
//}
//void Game::stateEnd() {
//	endstate = new EndState(this);
//	GameStateMachine::pushState(endstate);
//}
//
//void Game::gameExit(){
//	exit = true;
//}
//
//Game::~Game()
//{
//	// Elimina las texturas
//	for (Texture* texture : textures) delete texture;
//
//	// Desactiva la SDL
//	SDL_DestroyRenderer(renderer);
//	SDL_DestroyWindow(window);
//	SDL_Quit();
//}