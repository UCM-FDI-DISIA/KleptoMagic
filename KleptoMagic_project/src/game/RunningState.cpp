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
#include "../game/GhostComponent.h"
#include "../Class/SlimeComponents.h"
#include "../Class/TimerRenderer.h"
#include "../Class/UndeadArcherCMPS.h"
#include "../Class/MinigameGeneratorComponent.h"


//#include "../components/Health.h"
//#include "../components/Gun.h"

RunningState::RunningState(/*Manager* mgr) :_mngr(mgr*/) : _timer(300), minigame(nullptr) {
#ifdef _DEBUG
	std::cout << "Nuevo RunningState creado!" << std::endl;
#endif

	roomstorage = new RoomStorage();
	dungeonfloor = new DungeonFloor(10, 10, 10, 10, 10, roomstorage, sdlutils().renderer());
}
	

RunningState::~RunningState() {

}
bool RunningState::GMG(bool minigameActive) {
	ChestQuality chestQuality = ChestQuality::COMMON;
	MinigameGeneratorComponent _generatorA(&_timer, sdlutils().renderer());
	minigame = _generatorA.generateMinigame(chestQuality);
	minigame->start();
	minigame->minigameLogic(deltaTime);
	minigameActive = true;
	return minigameActive;
}

void RunningState::update() {
	
	bool exit = false;
	NewInputHandler::Instance()->init();
	
	TimerRenderer _timerRndr(&_timer, sdlutils().renderer());

	// reset the time before starting - so we calculate correct
	// delta-time in the first iteration
	//
	sdlutils().resetTime();
	Uint32 lastTime = sdlutils().currRealTime(); // Store initial time
	Uint32 currentTime = 0;
	deltaTime = 0.0f;

	bool minigameActive = false;

	while (!exit) {
		currentTime = sdlutils().currRealTime();			// Get the current time
		deltaTime = (currentTime - lastTime);
		lastTime = currentTime;								// Update lastTime for the next frame

		_timer.update(deltaTime);

		if (NewInputHandler::Instance()->isActionHeld(Action::ABILITY)) {
#ifdef _DEBUG
			std::cout << _timer.getTimeLeft() << std::endl;
#endif
		}

		if (NewInputHandler::Instance()->isActionPressed(Action::INTERACT)) {
			if (!minigameActive) {
				minigameActive = GMG(minigameActive);
			}
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

		// checking collisions
		colission_thisframe = false;
		checkCollisions();


		if (colission_thisframe )
		{
		
		
		}

		// clear screen
		sdlutils().clearRenderer();

			// render dungeon
			dungeonfloor->render();

		// render
		game().getMngr()->render();

		if (minigameActive) {
			minigame->minigameLogic(deltaTime);
			if (!minigame->running) {
				minigame->minigameLogic(deltaTime);
				minigameActive = false;
			}
		}

		_timerRndr.render(sdlutils().renderer(), _timer.getTimeLeft());

		// present new frame
		sdlutils().presentRenderer();

		Uint32 frameTime = sdlutils().currRealTime() - currentTime;

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
	game().getMngr()->addComponent<Image>(player, &sdlutils().images().at(selectedCharacter));
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
