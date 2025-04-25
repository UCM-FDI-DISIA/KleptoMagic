#include "RunningState.h"
#include "../game/PausedState.h"
#include "EnemyUtils.h"
#include "../utils/Collisions.h"
#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/NewInputHandler.h"
#include "../Class/Transform.h"
#include "../Class/TileCollisionChecker.h"
#include "../Class/MoveThroughRooms.h"
#include "../Class/Image.h"
#include "../Class/MovementCtrl.h"
#include "../Class/PlayerCtrl.h"
#include "../Class/PlayerAnimComponent.h"
#include "../game/GhostComponent.h"
#include "../Class/SlimeComponents.h"
#include "../Class/TimerCountdown.h"
#include "../Class/TimerRenderer.h"
#include "../Class/UndeadArcherCMPS.h"
#include "../Class/EntityStat.h"

//#include "../components/Health.h"
//#include "../components/Gun.h"

RunningState::RunningState(/*Manager* mgr) :_mngr(mgr*/) {
#ifdef _DEBUG
	std::cout << "Nuevo RunningState creado!" << std::endl;
#endif

	roomstorage = new RoomStorage();
	dungeonfloor = new DungeonFloor(10, 10, 10, 10, 10, roomstorage, sdlutils().renderer());
}
	

RunningState::~RunningState() {

}

void RunningState::update() {
	
	bool exit = false;
	NewInputHandler::Instance()->init();

	startTimeDelta = std::chrono::steady_clock::now();

	TimerCountdown _timer(300);
	_timer.start();
	TimerRenderer _timerRndr(&_timer, sdlutils().renderer());

	// reset the time before starting - so we calculate correct
	// delta-time in the first iteration
	//
	sdlutils().resetTime();

	while (!exit) {
		Uint32 startTime = sdlutils().currRealTime();
		_timer.update();

		if (NewInputHandler::Instance()->isActionHeld(Action::ABILITY)) {
#ifdef _DEBUG
			std::cout << _timer.getTimeLeft() << std::endl;
#endif
		}

		if (NewInputHandler::Instance()->isActionPressed(Action::INTERACT)) {

		}

		if (NewInputHandler::Instance()->isActionPressed(Action::PAUSE)) {
			game().pushState(new PausedState());
			exit = true;
		}
		
		if (NewInputHandler::Instance()->isActionHeld(Action::SHOOT))
		{
			bullet->pressed();

		}

		// update the event handler
		NewInputHandler::Instance()->update();

		// update
		game().getMngr()->update();
		game().getMngr()->refresh();
		bullet->update();
		dungeonfloor->update();

		// checking collisions
		colission_thisframe = false;
		checkCollisions();


		if (colission_thisframe )
		{
		
		
		}

		// clear screen
		sdlutils().clearRenderer(build_sdlcolor(0x000000FF));

			// render dungeon
			dungeonfloor->render();

		// render
		game().getMngr()->render();

		_timerRndr.render(sdlutils().renderer(), _timer.getTimeLeft());

		// present new frame
		sdlutils().presentRenderer();

		Uint32 frameTime = sdlutils().currRealTime() - startTime;

		if (frameTime < 20)
			SDL_Delay(20 - frameTime);
	}
}

void RunningState::checkCollisions() {

	auto _tr = game().getMngr()->getComponent<Transform>(game().getMngr()->getHandler(ecs::hdlr::PLAYER));
	////auto f_g = _mngr->getComponent<Gun>(_mngr->getHandler(ecs::hdlr::FIGHTER));
	//
	//// Iterate through asteroids

	for (auto enemy : game().getMngr()->getEntities(ecs::grp::ENEMY))
	{
		if(game().getMngr()->isAlive(enemy))
		{
			auto enemy_transform = game().getMngr()->getComponent<Transform>(enemy);
			if (Collisions::collides(
			_tr->getPos(),_tr->getWidth(),_tr->getHeight(),
		    enemy_transform->getPos(),enemy_transform->getWidth(),enemy_transform->getHeight()) && !colission_thisframe)
			{
				colission_thisframe = true;
				enemycolisioned = enemy;

			}
		}
	
	}
}

void RunningState::enter()
{
#ifdef _DEBUG
	std::cout << "Entrando en RunningState" << std::endl;
#endif
	//Player
	auto player = game().getMngr()->addEntity();
	game().getMngr()->setHandler(ecs::hdlr::PLAYER, player);
	auto tr = game().getMngr()->addComponent<Transform>(player);
	auto s = 50.0f;
	auto x = (sdlutils().width() - s) / 2.0f;
	auto y = (sdlutils().height() - s) / 2.0f;
	tr->init(Vector2D(x, y), Vector2D(), s, s, 0.0f);
	std::string selectedCharacter = game().getSelectedCharacter();
#ifdef _DEBUG
	std::cout << "Personaje seleccionado: " << selectedCharacter << std::endl;
#endif
	if (selectedCharacter.empty()) {
		selectedCharacter = "ALCHEMIST"; // Valor por defecto si no se ha seleccionado nada
	}
	//game().getMngr()->addComponent<Image>(player, &sdlutils().images().at(selectedCharacter));
	int charStartFrame;
	if (selectedCharacter == "KNIGHT") charStartFrame = 0;
	else if (selectedCharacter == "HUNTER") charStartFrame = 6;
	else if (selectedCharacter == "ROGUE") charStartFrame = 12;
	else if (selectedCharacter == "ALCHEMIST") charStartFrame = 18;
	game().getMngr()->addComponent<ImageWithFrames>(player, &sdlutils().images().at("player_sprites"), (float)500, 6, 4, charStartFrame, 1);
	game().getMngr()->addComponent<EntityStat>(player, 3, 1, 10, 1, 1);
	game().getMngr()->addComponent<PlayerCtrl>(player);
	auto tilechecker = game().getMngr()->addComponent<TileCollisionChecker>(player);
	tilechecker->init(false, tr, dungeonfloor);
	tr->initTileChecker(tilechecker);
	auto movethroughrooms = game().getMngr()->addComponent<MoveThroughRooms>(player);
	bullet = new BulletUtils();
	//bullet->addComponent(0);
	bullet->setDungeonFloor(dungeonfloor);
	movethroughrooms->init(dungeonfloor,bullet);
	movethroughrooms->enterRoom(' ');

	

	/*
	enemyutils().spawn_enemy(ENEMY_SLIME, Vector2D{ 100.0f, 100.0f });
	enemyutils().spawn_enemy(ENEMY_ARCHER, Vector2D{ 200.0f, 200.0f });
	enemyutils().spawn_enemy(ENEMY_ARMOR, Vector2D{ 300.0f, 300.0f });
	*/
}

void RunningState::leave()
{
}
