#include "RunningState.h"

#include "FighterUtils.h"
#include "AsteroidsUtils.h"
#include "../utils/Collisions.h"
#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/NewInputHandler.h"
#include "../Class/Transform.h"
#include "../Class/TileCollisionChecker.h"
#include "../Class/MoveThroughRooms.h"
#include "../Class/Image.h"
#include "../Class/MovementCtrl.h"
#include "../Class/PlayerCtrl.h"
#include "../Class/TimerCountdown.h"
#include "../Class/MinigameGeneratorComponent.h"
#include "../Class/Minigame.h"
#include "../Class/SlimeComponents.h"
#include "../Class/UndeadArcherCMPS.h"

//#include "../components/Health.h"
//#include "../components/Gun.h"

RunningState::RunningState(Manager* mgr) :_mngr(mgr) {
#ifdef _DEBUG
	std::cout << "Nuevo RunningState creado!" << std::endl;
#endif

	//asteroidSpawnTimer(sdlutils().virtualTimer()),
	//colission_thisframe(false);

	//asteroidSpawnTimer.resetTime();
	//fighterutils().create_fighter();

	roomstorage = new RoomStorage();
	dungeonfloor = new DungeonFloor(10, 10, 10, 10, 10, roomstorage, sdlutils().renderer());

	/*auto player = _mngr->addEntity();
	auto slime = _mngr->addEntity(ecs::grp::ENEMY);
	HomingComponent* comp;
	comp = new HomingComponent();
	//Player


	_mngr->setHandler(ecs::hdlr::PLAYER, player);
	auto tr = _mngr->addComponent<Transform>(player);
	auto s = 50.0f;
	auto x = (sdlutils().width() - s) / 2.0f;
	auto y = (sdlutils().height() - s) / 2.0f;
	tr->init(Vector2D(x, y), Vector2D(), s, s, 0.0f);
	//_mngr->addComponent<Image>(player, &sdlutils().images().at("ALCHEMIST"));
	std::string selectedCharacter = game().getSelectedCharacter();
	std::cout << "Personaje seleccionado: " << selectedCharacter << std::endl;
	if (selectedCharacter.empty()) {
		selectedCharacter = "ALCHEMIST"; // Valor por defecto si no se ha seleccionado nada
	}
	_mngr->addComponent<Image>(player, &sdlutils().images().at(selectedCharacter));
	_mngr->addComponent<PlayerCtrl>(player);
	 bullet = new Bullet();
	 bullet->addComponent(0);
	//Slime,
	auto slimetr = _mngr->addComponent<Transform>(slime);
	slimetr->init(Vector2D(x + 100, y - 100), Vector2D(), s, s, 0.0f);
	_mngr->addComponent<Image>(slime, &sdlutils().images().at("pacman"));
	_mngr->addComponent<SlimeVectorComponent>(slime);
	_mngr->addComponent<SlimeStatComponent>(slime);
	_mngr->addComponent<SlimeAttackComponent>(slime);
	_mngr->addComponent<SlimeMovementComponent>(slime);
	 bullet = new Bullet();

	//Archer

	_mngr->addComponent<SlimeMovementComponent>(slime);*/
}
	

RunningState::~RunningState() {

}

void RunningState::update() {
	
	bool exit = false;
	NewInputHandler::Instance()->init();

	startTimeDelta = std::chrono::steady_clock::now();

	TimerCountdown _timer(300);
	_timer.start();

	// reset the time before starting - so we calculate correct
	// delta-time in the first iteration
	//
	sdlutils().resetTime();

	while (!exit) {
		Uint32 startTime = sdlutils().currRealTime();
		_timer.update();

		if (NewInputHandler::Instance()->isActionHeld(Action::ABILITY)) {
			std::cout << _timer.getTimeLeft() << std::endl;
		}

		if (NewInputHandler::Instance()->isActionPressed(Action::INTERACT)) {
			std::cout << "Minigame creating" << std::endl;
			ChestQuality chestQuality = ChestQuality::COMMON;
			MinigameGeneratorComponent _generatorA(&_timer, sdlutils().renderer());
			Minigame* minigame = _generatorA.generateMinigame(chestQuality);

			if (minigame) {
				std::cout << "Minigame created" << std::endl;
				minigame->start(); // Start the generated minigame
				
				auto now = std::chrono::steady_clock::now();
				startTimeDelta = now;

				while (minigame->running) {
					auto now = std::chrono::steady_clock::now();
					auto DeltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTimeDelta).count();
					_timer.update();
					//std::cout << _timer.getTimeLeft() << std::endl;
					NewInputHandler::Instance()->update();
					minigame->minigameLogic(DeltaTime);
					startTimeDelta = now;
				}
				minigame->minigameLogic(DeltaTime);
			}
		}

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

		//}
		/*

		if (ihdlr.isKeyDown(SDL_SCANCODE_SPACE)) {

			bullet->pressed();
		}if (ihdlr.isKeyDown(SDL_SCANCODE_K)) {

			bullet->changeAttSpeedCapMul(-0.05);
		}
			*/
		
		if (NewInputHandler::Instance()->isActionHeld(Action::SHOOT))
		{
			bullet->pressed();

		}

		// update the event handler
		NewInputHandler::Instance()->update();

		// update fighter and asteroids here
		_mngr->update();
		_mngr->refresh();

		// checking collisions
		colission_thisframe = false;
		checkCollisions();


		if (colission_thisframe )
		{
		
		
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

			// render dungeon
			dungeonfloor->render();

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
				enemycolisioned = enemy;

			}
		}
	
	}

	

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
#ifdef _DEBUG
	std::cout << "Entrando en RunningState" << std::endl;
#endif



	auto player = _mngr->addEntity();

	auto slime = _mngr->addEntity(ecs::grp::ENEMY);
	auto archer = _mngr->addEntity(ecs::grp::ENEMY);

	//Player
	_mngr->setHandler(ecs::hdlr::PLAYER, player);

	auto tr = _mngr->addComponent<Transform>(player);
	auto s = 50.0f;
	auto x = (sdlutils().width() - s) / 2.0f;
	auto y = (sdlutils().height() - s) / 2.0f;
	tr->init(Vector2D(x, y), Vector2D(), s, s, 0.0f);
	//_mngr->addComponent<Image>(player, &sdlutils().images().at("ALCHEMIST"));
	std::string selectedCharacter = game().getSelectedCharacter();
	std::cout << "Personaje seleccionado: " << selectedCharacter << std::endl;
	if (selectedCharacter.empty()) {
		selectedCharacter = "ALCHEMIST"; // Valor por defecto si no se ha seleccionado nada
	}
	_mngr->addComponent<Image>(player, &sdlutils().images().at(selectedCharacter));
	_mngr->addComponent<PlayerCtrl>(player);
	auto tilechecker = _mngr->addComponent<TileCollisionChecker>(player);
	tilechecker->init(false, tr, dungeonfloor);
	tr->initTileChecker(tilechecker);
	auto movethroughrooms = _mngr->addComponent<MoveThroughRooms>(player);
	movethroughrooms->init(dungeonfloor);
	movethroughrooms->enterRoom(' ');

	bullet = new Bullet();
	bullet->addComponent(0);
	//Slime,
	//_mngr->setHandler(ecs::hdlr::SLIME, slime);
	auto slimetr = _mngr->addComponent<Transform>(slime);
	slimetr->init(Vector2D(x + 100, 5 - 20), Vector2D(), s, s, 0.0f);
	_mngr->addComponent<Image>(slime, &sdlutils().images().at("pacman"));
	_mngr->addComponent<SlimeVectorComponent>(slime);
	_mngr->addComponent<SlimeStatComponent>(slime);
	_mngr->addComponent<SlimeAttackComponent>(slime);
	_mngr->addComponent<SlimeMovementComponent>(slime);

	auto archertr = _mngr->addComponent<Transform>(archer);
	archertr->init(Vector2D(x + 70, y - 100), Vector2D(), s, s, 0.0f);
	_mngr->addComponent<Image>(archer, &sdlutils().images().at("star"));
	_mngr->addComponent<UndeadStatComponent>(archer);
	_mngr->addComponent<UndeadVectorComponent>(archer);
	_mngr->addComponent<UndeadMovementComponent>(archer);
	_mngr->addComponent<UndeadAttackComponent>(archer);

}

void RunningState::leave()
{
}
