#include "RunningState.h"

#include "FighterUtils.h"
#include "AsteroidsUtils.h"
#include "../utils/Collisions.h"
#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/InputHandler.h"
#include "../Class/Transform.h"
#include "../Class/Image.h"
#include "../Class/MovementCtrl.h"
#include "../Class/PlayerCtrl.h"
//#include "../components/Health.h"
//#include "../components/Gun.h"

RunningState::RunningState(Manager* mgr) :_mngr(mgr) {
	//asteroidSpawnTimer(sdlutils().virtualTimer()),
	//colission_thisframe(false);

	//asteroidSpawnTimer.resetTime();
	//fighterutils().create_fighter();
	auto player = _mngr->addEntity();
	_mngr->setHandler(ecs::hdlr::PLAYER, player);
	auto tr = _mngr->addComponent<Transform>(player);
	auto s = 50.0f;
	auto x = (sdlutils().width() - s) / 2.0f;
	auto y = (sdlutils().height() - s) / 2.0f;
	tr->init(Vector2D(x, y), Vector2D(), s, s, 0.0f);
	_mngr->addComponent<Image>(player, &sdlutils().images().at("player"));
	_mngr->addComponent<PlayerCtrl>(player);
	 bullet = new Bullet();
	bullet->generateBullets();

}
	

RunningState::~RunningState() {

}

void RunningState::update() {
	
	bool exit = false;
	auto& ihdlr = ih();

	// reset the time before starting - so we calculate correct
	// delta-time in the first iteration
	//
	sdlutils().resetTime();

	while (!exit) {
		Uint32 startTime = sdlutils().currRealTime();

		// update the event handler
		ih().refresh();

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
		//}
		if (ihdlr.isKeyDown(SDL_SCANCODE_K)) {

			bullet->pressed();
		}
		// update fighter and asteroids here
		_mngr->update();
		_mngr->refresh();

		// checking collisions
		colission_thisframe = false;
		checkCollisions();

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

	//auto f_t = _mngr->getComponent<Transform>(_mngr->getHandler(ecs::hdlr::FIGHTER));
	////auto f_g = _mngr->getComponent<Gun>(_mngr->getHandler(ecs::hdlr::FIGHTER));
	//
	//// Iterate through asteroids
	//for (auto a : _mngr->getEntities(ecs::grp::ASTEROIDS)) {
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
