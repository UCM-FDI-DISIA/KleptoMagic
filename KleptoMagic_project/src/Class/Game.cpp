#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "gameStateMachine.h"
#include "Game.h"
#include "DummyState.h"
#include "MainMenuState.h"
#include "PlayState.h"
#include "../sdlutils/InputHandler.h"

//componentes :D
#include "../ecs/Manager.h"

//states
#include "../game/RunningState.h"
#include "../game/NewGameState.h"
#include "../game/NewRoundState.h"
#include "../game/PausedState.h"
#include "../game/GameOverState.h"

using namespace std;

// Formato de la especificación de una textura
struct TextureSpec
{
	const char* name;	// Ruta del archivo
	uint numColumns;	// Número de frames por fila
	uint numRows;		// Número de frames por columna
};

// Directorio raíz de los archivos de textura
const string textureRoot = "../KleptoMagic_project/resources/images/";

// Especificación de las texturas del juego
const array<TextureSpec, Game::NUM_TEXTURES> textureSpec{
	TextureSpec{"endmenu-provisional.png", 1, 1},
	TextureSpec{"return-button.png", 1, 1},
	TextureSpec{"player_placeholder.png",1,1}
};

Game::Game() : exit(false), _mngr(nullptr), _state(nullptr){}

bool Game::init() {
	if (!SDLUtils::Init("KleptoMagic", 800, 600, "resources/config/resources.json")) {
		std::cerr << "Error inicializando SDLUtils" << std::endl;
		return false;
	}

	if (!InputHandler::Init()) {
		std::cerr << "Error inicializando InputHandler" << std::endl;
		return false;
	}

	return true;

	/*
	// Inicializa la SDL
	SDL_Init(SDL_INIT_EVERYTHING);
	window = SDL_CreateWindow("First test with SDL",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WIN_WIDTH,
		WIN_HEIGHT,
		SDL_WINDOW_SHOWN);

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	if (window == nullptr || renderer == nullptr)
		throw "Error cargando SDL"s;




	// Carga las texturas
	/*for (int i = 0; i < NUM_TEXTURES; ++i)
		textures[i] = new Texture(renderer,
			(textureRoot + textureSpec[i].name).c_str()); * /
	for (int i = 0; i < NUM_TEXTURES; ++i) {
		std::string texturePath = textureRoot + textureSpec[i].name;
#ifdef _DEBUG
		std::cout << "Cargando textura: " << texturePath << std::endl;
#endif
		textures[i] = new Texture(renderer, texturePath);
		if (textures[i] == nullptr) {
#ifdef _DEBUG
			std::cerr << "Error: No se pudo cargar la textura " << texturePath << std::endl;
#endif
		}
	}


	//dummy = new DummyState();
	//GameStateMachine::pushState(dummy);
	// Creación de playstates
	playstate = new PlayState(this); //se fue a su metodo propio

	_inputManager = new InputManager();
	mainmenu = new MainMenuState(this, textures[Game::MAINMENUBACKGROUND]);
	GameStateMachine::pushState(mainmenu);
	//GameStateMachine::pushState(playstate);

	//mainmenu = new MainMenuState(this, textures[Game::BACKGROUND]);
	//GameStateMachine::pushState(mainmenu);
	GameStateMachine::pushState(playstate);
	*/
}

bool Game::initGame() {
	_mngr = new Manager();

	//if (!AsteroidsUtils::Init(_mngr)) {
	//	std::cerr << "Error inicializando AsteroidsUtils" << std::endl;
	//	return false;
	//}

	//if (!FighterUtils::Init(_mngr)) {
	//	std::cerr << "Error inicializando FighterUtils" << std::endl;
	//	return false;
	//}
	_running_state = new RunningState(_mngr);
	_gameover_state = new GameOverState();
	_newgame_state = new NewGameState();
	_newround_state = new NewRoundState();
	_paused_state = new PausedState();
	//fighterutils().create_fighter();
	//asteroidsutils().create_asteroids(10);

	_state = _newgame_state;

	auto ginfo = _mngr->addEntity();
	_mngr->setHandler(ecs::hdlr::GAMEINFO, ginfo);
	//_mngr->addComponent<GameCtrl>(ginfo);

	return true;
}

Game::~Game() {
	delete _mngr;

	// release InputHandler if the instance was created correctly.
	if (InputHandler::HasInstance())
		InputHandler::Release();

	// release SLDUtil if the instance was created correctly.
	if (SDLUtils::HasInstance())
		SDLUtils::Release();

	// release AsteroidsUtils if the instance was created correctly.
	//if (AsteroidsUtils::HasInstance())
	//	AsteroidsUtils::Release();
}

void Game::start() {
	bool exit = false;
	auto& ihdlr = ih();
	auto& vt = sdlutils().virtualTimer();

	vt.resetTime();

	while (!exit) {
		std::cout << "Entrando en el bucle principal" << std::endl;
		Uint32 startTime = vt.regCurrTime();
		ihdlr.refresh();

/*
		//SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);

		SDL_Event evento;
		GameStateMachine::handleEvent(evento);
		while (SDL_PollEvent(&evento)) {
			if (evento.type == SDL_QUIT)
				exit = true;
			else {
				GameStateMachine::handleEvent(evento);
			}
*/
		if (ihdlr.isKeyDown(SDL_SCANCODE_ESCAPE)) {
			exit = true;
			continue;
		}
		std::cout << "Exit: " << exit << std::endl;


		//std::cout << _state << std::endl;
		_state->update();
		std::cout << "Se ejecutó update()" << std::endl;


		Uint32 frameTime = sdlutils().currRealTime() - startTime;
		if (frameTime < 10) SDL_Delay(10 - frameTime);
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