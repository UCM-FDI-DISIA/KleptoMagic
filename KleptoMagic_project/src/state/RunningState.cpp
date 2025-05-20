#include "RunningState.h"
#include "../state/PausedState.h"
#include "../state/GameOverState.h"
#include "../enemies/EnemyUtils.h"
#include "../utils/Collisions.h"
#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/NewInputHandler.h"
#include "../ecs/Transform.h"
#include "../room/TileCollisionChecker.h"
#include "../room/MoveThroughRooms.h"
#include "../render/Image.h"
#include "../player/MovementCtrl.h"
#include "../player/PlayerCtrl.h"
#include "../player/PlayerAnimComponent.h"
#include "../render/TimerRenderer.h"
#include "../enemies/GhostComponent.h"
#include "../enemies/SlimeComponents.h"
#include "../enemies/UndeadArcherCMPS.h"
#include "../room/MinigameGeneratorComponent.h"
#include "../ecs/EntityStat.h"
#include "../room/PickableCMP.h"

RunningState::RunningState() : _timer(300), minigame(nullptr) {
#ifdef _DEBUG
	std::cout << "Nuevo RunningState creado!" << std::endl;
#endif
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

	// Posiones imagen
	int texW = controlsTexture->width();
	int texH = controlsTexture->height();

	float scale = 0.3f;

	int scaledW = static_cast<int>(texW * scale);
	int scaledH = static_cast<int>(texH * scale);
	int x = (sdlutils().width() - scaledW) / 2;
	int y = (sdlutils().height() - scaledH) / 2;

	SDL_Rect dest = { x, y, scaledW, scaledH };

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
		dungeonfloor->update();

		// checking collisions
		colission_thisframe = false;
		checkCollisions();

		// clear screen
		sdlutils().clearRenderer(build_sdlcolor(0x000000FF));

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

		// Si han pasado mas de 10 segundos, ocultamos la imagen de controles
		if (_timer.getTimeLeft() >= 290) {
			controlsTexture->setAlpha(128);  
			controlsTexture->render(dest);
		}

		// Mostrar corazones seg�n la vida del jugador
		auto player = game().getMngr()->getHandler(ecs::hdlr::PLAYER);
		if (player != nullptr && game().getMngr()->isAlive(player)) {
			auto stats = game().getMngr()->getComponent<EntityStat>(player);
			hp = stats->getStat(EntityStat::Stat::HealthCurrent);
			float hpTotal = stats->getStat(EntityStat::Stat::HealthTotal);

			int maxHearts = static_cast<int>(hpTotal);
			int currentHearts = static_cast<int>(hp);

			/*#ifdef _DEBUG
						std::cout << "HealthCurrent: " << hp << std::endl;
			#endif*/

			int heartSize = 64;
			int spacing = 10;

			// Calculamos la posicion X desde la derecha
			int totalWidth = maxHearts * heartSize + (maxHearts - 1) * spacing;
			int startX = sdlutils().width() - totalWidth - 10;
			int startY = 10;

			SDL_Rect heartDest = { startX, startY, heartSize, heartSize };

			for (int i = 0; i < maxHearts; ++i) {
				if (i < currentHearts) {
					// Corazon lleno
					if (hearthTexture != nullptr)
						hearthTexture->render(heartDest);
				}
				else {
					// Corazon vacio
					if (hearthTotalTexture != nullptr)
						hearthTotalTexture->render(heartDest);
				}
				heartDest.x += heartSize + spacing;
			}
		}

		if (hp <= 0 || _timer.getTimeLeft() <= 0)
		{
			game().pushState(new GameOverState());
			exit = true;
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

	auto _mngr = game().getMngr();
	auto _tr = _mngr->getComponent<Transform>(game().getMngr()->getHandler(ecs::hdlr::PLAYER));

	for (auto enemy : game().getMngr()->getEntities(ecs::grp::ENEMY))
	{
		if(_mngr->isAlive(enemy))
		{
			auto* enemy_transform = _mngr->getComponent<Transform>(enemy);
			if (Collisions::collides(
			_tr->getPos(),_tr->getWidth(),_tr->getHeight(),
		    enemy_transform->getPos(),enemy_transform->getWidth(),enemy_transform->getHeight()) && !colission_thisframe)
			{
				colission_thisframe = true;
			}

			for (auto bullet : _mngr->getEntities(ecs::grp::BULLET)) {
				auto bullet_tr = _mngr->getComponent<Transform>(bullet);

				if(Collisions::collides(
					enemy_transform->getPos(), enemy_transform->getWidth(), enemy_transform->getHeight(),
					bullet_tr->getPos(), bullet_tr->getWidth(), bullet_tr->getHeight()) && !colission_thisframe) 
				{
					auto* enemy_stats = _mngr->getComponent<EntityStat>(enemy);
					auto* bullet_stats = _mngr->getComponent<BulletStats>(bullet);

					enemy_stats->ChangeStat(-1 * bullet_stats->getDamage(), EntityStat::Stat::HealthCurrent);
				}
			}
		}
	}

	for (auto* upgrade : game().getMngr()->getEntities(ecs::grp::UPGRRADE)) 
	{
		if(game().getMngr()->isAlive(upgrade))
		{
			auto* upgrade_cmp = game().getMngr()->getComponent<PickableCMP>(upgrade);
			upgrade_cmp->playerCollision();
			game().getMngr()->setAlive(upgrade, false);
		}
	}
}

void RunningState::enter()
{
#ifdef _DEBUG
	std::cout << "Entrando en RunningState" << std::endl;
#endif

	if (controlsTexture == nullptr) {
		controlsTexture = new Texture(sdlutils().renderer(), "resources/images/controles.png");
	}

	if (hearthTexture == nullptr) {
		hearthTexture = new Texture(sdlutils().renderer(), "resources/images/live.png");
	}

	if (hearthTotalTexture == nullptr) {
		hearthTotalTexture = new Texture(sdlutils().renderer(), "resources/images/liveEmpty.png");
	}

	auto player = game().getMngr()->getHandler(ecs::hdlr::PLAYER);

	if (player == nullptr || !game().getMngr()->isAlive(player)) {
		roomstorage = new RoomStorage();
		dungeonfloor = new DungeonFloor(10, 10, 10, 10, 10, roomstorage, sdlutils().renderer());
		auto s = 50.0f;
		auto x = (sdlutils().width() - s) / 2.0f;
		auto y = (sdlutils().height() - s) / 2.0f;
		auto pos = Vector2D(x, y);
		bullet = new BulletUtils();
		//bullet->addComponent(0);
		bullet->setDungeonFloor(dungeonfloor);
		playerutils().createPlayer(pos, s, bullet);
	}
}

void RunningState::leave()
{
}
