#include "RunningState.h"

#include "FighterUtils.h"
#include "AsteroidsUtils.h"
#include "../utils/Collisions.h"
#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/NewInputHandler.h"
#include "../Class/Transform.h"
#include "../Class/Image.h"
#include "../Class/MovementCtrl.h"
#include "../Class/PlayerCtrl.h"
#include "../Class/TimerCountdown.h"
#include "../Class/MinigameGeneratorComponent.h"
#include "../Class/Minigame.h"
#include "../Class/SlimeComponents.h"
//#include "../components/Health.h"
//#include "../components/Gun.h"

RunningState::RunningState(Manager* mgr) :_mngr(mgr), renderer(nullptr) {
	//asteroidSpawnTimer(sdlutils().virtualTimer()),
	//colission_thisframe(false);

	//asteroidSpawnTimer.resetTime();
	//fighterutils().create_fighter();
	auto player = _mngr->addEntity();
	auto slime = _mngr->addEntity(ecs::grp::ENEMY);

	//Player
	_mngr->setHandler(ecs::hdlr::PLAYER, player);
	auto tr = _mngr->addComponent<Transform>(player);
	auto s = 50.0f;
	auto x = (sdlutils().width() - s) / 2.0f;
	auto y = (sdlutils().height() - s) / 2.0f;
	tr->init(Vector2D(x, y), Vector2D(), s, s, 0.0f);
	_mngr->addComponent<Image>(player, &sdlutils().images().at("player"));
	_mngr->addComponent<PlayerCtrl>(player);

	//Slime,
	_mngr->setHandler(ecs::hdlr::SLIME, slime);
	auto slimetr = _mngr->addComponent<Transform>(slime);
	slimetr->init(Vector2D(x + 100, 5 - 20), Vector2D(), s, s, 0.0f);
	_mngr->addComponent<Image>(slime, &sdlutils().images().at("pacman"));
	_mngr->addComponent<SlimeVectorComponent>(slime);
	_mngr->addComponent<SlimeStatComponent>(slime);
	_mngr->addComponent<SlimeAttackComponent>(slime);
	_mngr->addComponent<SlimeMovementComponent>(slime);

}
	

RunningState::~RunningState() {
	if (renderer) {
		SDL_DestroyRenderer(renderer);
	}
}

void RunningState::update() {
	
	bool exit = false;
	NewInputHandler::Instance()->init();

	TimerCountdown _timer(300);
	_timer.start();

	// reset the time before starting - so we calculate correct
	// delta-time in the first iteration
	//
	sdlutils().resetTime();

	while (!exit) {
		Uint32 startTime = sdlutils().currRealTime();
		_timer.update();





		// Initialize SDL
		if (SDL_Init(SDL_INIT_VIDEO) < 0) {
			std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
			return;
		}

		// Create SDL_Window
		SDL_Window* window = SDL_CreateWindow(
			"Game Title",                      // Title
			SDL_WINDOWPOS_CENTERED,            // X position
			SDL_WINDOWPOS_CENTERED,            // Y position
			800,                               // Width
			600,                               // Height
			SDL_WINDOW_SHOWN                   // Flags
		);

		if (!window) {
			std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
			SDL_Quit();
			return;
		}

		// Create SDL_Renderer
		SDL_Renderer* renderer = SDL_CreateRenderer(
			window,
			-1,
			SDL_RENDERER_ACCELERATED
		);

		if (!renderer) {
			std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
			SDL_DestroyWindow(window);
			SDL_Quit();
			return;
		}







		if (NewInputHandler::Instance()->isActionPressed(Action::PAUSE)) {
			//Game::Instance()->setState();
		}

		if (NewInputHandler::Instance()->isActionPressed(Action::INTERACT)) {
			ChestQuality chestQuality = ChestQuality::COMMON;
			MinigameGeneratorComponent _generatorA(&_timer, renderer);
			Minigame* minigame = _generatorA.generateMinigame(chestQuality);

			if (minigame) {
				minigame->start(); // Start the generated minigame
			}
		}

		// update the event handler
		NewInputHandler::Instance()->update();

		// if 0 asteroids change to GameOverState
		//if (asteroidsutils().count_asteroids() <= 0) {
		//	// here
		//	game().setState(Game::GAMEOVER);
		//	exit = true;
		//}
		//
		//// if pressed P enter PauseState
		//if (ihdlr.isKeyDown(SDL_SCANCODE_P)) {
		//	// here
		//	game().setState(Game::PAUSED);
		//	exit = true;
		//

		// update fighter and asteroids here
		_mngr->update();
		_mngr->refresh();

		// checking collisions
		colission_thisframe = false;
		checkCollisions();

		if (colission_thisframe)
		{
			for (auto enemy : _mngr->getEntities(ecs::grp::ENEMY))
			{
				if (_mngr->isAlive(enemy))
				{	
					if(enemy->getMngr()->hasComponent<SlimeAttackComponent>(enemy)) 
					{
						enemy->getMngr()->getComponent<SlimeAttackComponent>(enemy)->Colision();
					}									
				}

			}
		}
			//if (colission_thisframe) {
			//	fighterutils().take_life();
			//	if (fighterutils().get_lives() > 0) {
			//		game().setState(Game::NEWROUND);
			//	}
			//	else {
			//		game().setState(Game::GAMEOVER);
			//	}
			//	exit = true;
			//}

			// clear screen
			sdlutils().clearRenderer();

			// render
			_mngr->render();

			// present new frame
			sdlutils().presentRenderer();

			// spawn new asteroid every 5s
			//if (asteroidSpawnTimer.currRealTime() >= asteroidSpawnCDms) {
			//	asteroidSpawnTimer.resetTime();
			//	asteroidsutils().create_asteroids(1); // AJUSTE: Asteroides spawneando cada 5s
			//}

			Uint32 frameTime = sdlutils().currRealTime() - startTime;

			if (frameTime < 20)
				SDL_Delay(20 - frameTime);
		}
	
}

void RunningState::checkCollisions() {

	auto _tr = _mngr->getComponent<Transform>(_mngr->getHandler(ecs::hdlr::PLAYER));
	////auto f_g = _mngr->getComponent<Gun>(_mngr->getHandler(ecs::hdlr::FIGHTER));
	//
	//// Iterate through asteroids
	for (auto enemy : _mngr->getEntities(ecs::grp::ENEMY))
	{
		if(_mngr->isAlive(enemy))
		{
			auto enemy_transform = _mngr->getComponent<Transform>(enemy);
			if (Collisions::collides(
			_tr->getPos(),_tr->getWidth(),_tr->getHeight(),
		    enemy_transform->getPos(),enemy_transform->getWidth(),enemy_transform->getHeight()) && !colission_thisframe)
			{
				colission_thisframe = true;

			}
		}
	
	}

	
	//	if (_mngr->isAlive(a)) {
	//		auto a_t = _mngr->getComponent<Transform>(a);
	//
	//		// With fighter
	//		if (Collisions::collidesWithRotation(
	//			f_t->getPos(), f_t->getWidth(), f_t->getHeight(), f_t->getRot(),
	//			a_t->getPos(), a_t->getWidth(), a_t->getHeight(), a_t->getRot()
	//			) && !colission_thisframe) {
	//			colission_thisframe = true;
	//		}
	//
	//		// With bullets
	//		//for (Gun::Bullet& b : *f_g) {
	//		//	if (b.used) {
	//		//		if (Collisions::collidesWithRotation(
	//		//			a_t->getPos(), a_t->getWidth(), a_t->getHeight(), a_t->getRot(),
	//		//			b.pos, b.width, b.height, b.rot
	//		//		)) {
	//		//			b.used = false;
	//		//			asteroidsutils().split_asteroid(a);
	//		//		}
	//		//	}
	//		//}
	//	}
	//}


}

void RunningState::enter()
{
}

void RunningState::leave()
{
}
