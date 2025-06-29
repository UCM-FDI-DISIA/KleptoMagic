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
#include "../enemies/SlimeComponents.h"
#include "../enemies/UndeadArcherCMPS.h"
#include "../room/MinigameGeneratorComponent.h"
#include "../ecs/EntityStat.h"
#include "../room/PickableCMP.h"
#include "../render/Camera.h"
#include "../enemies/BossCMP.h"

#include "../utils/checkML.h"

//#include "../components/Health.h"
//#include "../components/Gun.h"

RunningState::RunningState() : _timer(300), minigame(nullptr) {
#ifdef _DEBUG
	std::cout << "Nuevo RunningState creado!" << std::endl;
#endif
	//Image* im = new Image();
}


RunningState::~RunningState() {
	// Liberar texturas
	if (controlsTexture != nullptr) {
		delete controlsTexture;
		controlsTexture = nullptr;
	}
	if (hearthTexture != nullptr) {
		delete hearthTexture;
		hearthTexture = nullptr;
	}
	if (hearthTotalTexture != nullptr) {
		delete hearthTotalTexture;
		hearthTotalTexture = nullptr;
	}

	// Liberar música
	if (gameBGM != nullptr) {
		Mix_FreeMusic(gameBGM);
		gameBGM = nullptr;
	}

	// Liberar otros recursos
	if (bullet != nullptr) {
		delete bullet;
		bullet = nullptr;
	}
	if (roomstorage != nullptr) {
		delete roomstorage;
		roomstorage = nullptr;
	}
	if (dungeonfloor != nullptr) {
		delete dungeonfloor;
		dungeonfloor = nullptr;
	}
	if (itemStorage != nullptr) {
		delete itemStorage;
		//itemStorage = nullptr;
	}
	if (minigame != nullptr) {
		delete minigame;
		minigame = nullptr;
	}
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

		auto player = game().getMngr()->getHandler(ecs::hdlr::PLAYER);

		if (player != nullptr && game().getMngr()->isAlive(player)) {
			auto tr = game().getMngr()->getComponent<Transform>(player);
			Vector2D playerCenter = tr->getPos() + Vector2D(tr->getWidth() / 2, tr->getHeight() / 2);
			camOffset = playerCenter - Vector2D(sdlutils().width() / 2, sdlutils().height() / 2);

#ifdef _DEBUG
			//std::cout << "camOffset: " << camOffset.getX() << ", " << camOffset.getY() << std::endl;
#endif // _DEBUG

		}

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

		if (player != nullptr && game().getMngr()->isAlive(player)) {
			auto stats = game().getMngr()->getComponent<EntityStat>(player);
			hp = stats->getStat(EntityStat::Stat::HealthCurrent);
			float hpTotal = stats->getStat(EntityStat::Stat::HealthTotal);

			int maxHearts = static_cast<int>(hpTotal);
			int currentHearts = static_cast<int>(hp);
#ifdef _DEBUG
			//std::cout << "HealthTotal: " << hpTotal << std::endl;
#endif
#ifdef _DEBUG
			//std::cout << "HealthCurrent: " << hp << std::endl;
#endif

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

		bool bossDefeated = false;
		for (auto enemy : game().getMngr()->getEntities(ecs::grp::ENEMY)) {
			if (game().getMngr()->hasComponent<BossStatComponent>(enemy)) {
				auto stats = game().getMngr()->getComponent<EntityStat>(enemy);
				if (stats != nullptr && stats->isDead()) {
					bossDefeated = true;
					break;
				}
			}
		}

		if (hp <= 0 || _timer.getTimeLeft() <= 0)
		{
			game().setEndResult(false);
			game().pushState(new GameOverState());
			exit = true;
		}
		else if (bossDefeated) {
			game().setEndResult(true);
			game().pushState(new GameOverState());
			exit = true;
		}

		_timerRndr.render(sdlutils().renderer(), _timer.getTimeLeft());

		renderPlayerStats();

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
		if (_mngr->isAlive(enemy))
		{
			auto* enemy_transform = _mngr->getComponent<Transform>(enemy);
			if (Collisions::collides(
				_tr->getPos(), _tr->getWidth(), _tr->getHeight(),
				enemy_transform->getPos(), enemy_transform->getWidth(), enemy_transform->getHeight()) && !colission_thisframe)
			{
				colission_thisframe = true;
			}

			for (auto bullets : _mngr->getEntities(ecs::grp::BULLET)) {
				auto bullet_tr = _mngr->getComponent<Transform>(bullets);

				if (Collisions::collides(
					enemy_transform->getPos(), enemy_transform->getWidth(), enemy_transform->getHeight(),
					bullet_tr->getPos(), bullet_tr->getWidth(), bullet_tr->getHeight()) && !colission_thisframe)
				{
					auto* enemy_stats = _mngr->getComponent<EntityStat>(enemy);
					auto* bullet_stats = _mngr->getComponent<BulletStats>(bullets);
					enemy_stats->ChangeStat(-1 * bullet_stats->getDamage(), EntityStat::Stat::HealthCurrent);
					bullet->collided(bullets);
				}
			}
		}
	}
	for (auto bullets : _mngr->getEntities(ecs::grp::ENEMYBULLET)) {
		auto bullet_tr = _mngr->getComponent<Transform>(bullets);

		if (Collisions::collides(
			_tr->getPos(), _tr->getWidth(), _tr->getHeight(),
			bullet_tr->getPos(), bullet_tr->getWidth(), bullet_tr->getHeight()) && !colission_thisframe)
		{
			//auto* enemy_stats = _mngr->getComponent<EntityStat>(pla);
			auto* player_stats = _mngr->getComponent<EntityStat>(_mngr->getHandler(ecs::hdlr::PLAYER));
			auto* bullet_stats = _mngr->getComponent<BulletStats>(bullets);
			auto* bullet_hitted = _mngr->getComponent<PlayerHitted>(bullets);
			if (!bullet_stats->getPiercing() || (bullet_hitted != nullptr && bullet_hitted->AddPlayer())) {
				player_stats->ChangeStat(-1 * bullet_stats->getDamage(), EntityStat::Stat::HealthCurrent);
				game().playHurtSound();
				bullet->collided(bullets);
			}
		}

	}
	for (auto* upgrade : game().getMngr()->getEntities(ecs::grp::OBJECT))
	{
		if (game().getMngr()->isAlive(upgrade))
		{
			auto* upgrade_transform = _mngr->getComponent<Transform>(upgrade);

			if (Collisions::collides(
				_tr->getPos(), _tr->getWidth(), _tr->getHeight(),
				upgrade_transform->getPos(), upgrade_transform->getWidth(), upgrade_transform->getHeight()) && !colission_thisframe)
			{
				auto* upgrade_cmp = game().getMngr()->getComponent<PickableCMP>(upgrade);
				upgrade_cmp->playerCollision();
			}


		}
	}
}

void RunningState::enter()
{
#ifdef _DEBUG
	std::cout << "Entrando en RunningState" << std::endl;
#endif

	// Inicializar la música si no se ha hecho ya
	if (gameBGM == nullptr) {
		gameBGM = Mix_LoadMUS("resources/sound/gameBGM.mp3");
		if (gameBGM == nullptr) {
#ifdef _DEBUG
			std::cerr << "Error cargando la música: " << Mix_GetError() << std::endl;
#endif
		}
	}

	// Reproducir la música si no se ha empezado ya
	if (gameBGM != nullptr) {
		Mix_VolumeMusic(38);
		Mix_PlayMusic(gameBGM, 0); // 0 significa que no se repetirá
	}

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
		itemStorage = new ItemStorage("resources/item_data/objetos.txt");
		dungeonfloor = new DungeonFloor(10, 10, 10, 10, 10, roomstorage, sdlutils().renderer());
		ObjectUtils::Instance()->updateStorage(itemStorage);
		auto s = 50.0f;
		auto x = (sdlutils().width() - s) / 2.0f;
		auto y = (sdlutils().height() - s) / 2.0f;
		auto pos = Vector2D(x, y);
		bullet = new BulletUtils();
		//bullet->addComponent(0);
		bullet->setDungeonFloor(dungeonfloor);
		playerutils().createPlayer(pos, s, bullet);
		ObjectUtils::Instance()->setBulletUtils(bullet);
	}
	camOffset = Vector2D(0, 0);
}

void RunningState::renderPlayerStats() {
	auto player = game().getMngr()->getHandler(ecs::hdlr::PLAYER);
	if (player != nullptr && game().getMngr()->isAlive(player)) {
		auto stats = game().getMngr()->getComponent<EntityStat>(player);

		// Configuración de posición y estilo
		int startX = 20;  // Margen izquierdo
		int startY = sdlutils().height() - 120;  // 120 píxeles desde abajo
		int lineHeight = 30;  // Espacio entre líneas
		SDL_Color textColor = { 255, 255, 255, 255 };  // Blanco
		SDL_Color highlightColor = { 255, 215, 0, 255 };  // Color dorado para valores

		// Obtener los valores de los stats
		float shield = stats->getStat(EntityStat::Stat::Shield);
		float movementSpeed = stats->getStat(EntityStat::Stat::MovementSpeed);
		float attackSpeed = stats->getStat(EntityStat::Stat::AttackSpeed);
		float damage = stats->getStat(EntityStat::Stat::Damage);

		// Crear texturas temporales para cada elemento de texto
		Texture shieldLabel(sdlutils().renderer(), "SHIELD:", sdlutils().fonts().at("ARIAL16"), textColor);
		Texture shieldValue(sdlutils().renderer(), std::to_string((int)shield), sdlutils().fonts().at("ARIAL24"), highlightColor);

		Texture speedLabel(sdlutils().renderer(), "SPEED:", sdlutils().fonts().at("ARIAL16"), textColor);
		Texture speedValue(sdlutils().renderer(), std::to_string((int)movementSpeed), sdlutils().fonts().at("ARIAL24"), highlightColor);

		Texture atkSpeedLabel(sdlutils().renderer(), "ATK SPD:", sdlutils().fonts().at("ARIAL16"), textColor);
		Texture atkSpeedValue(sdlutils().renderer(), std::to_string((int)attackSpeed), sdlutils().fonts().at("ARIAL24"), highlightColor);

		Texture damageLabel(sdlutils().renderer(), "DAMAGE:", sdlutils().fonts().at("ARIAL16"), textColor);
		Texture damageValue(sdlutils().renderer(), std::to_string((int)damage), sdlutils().fonts().at("ARIAL24"), highlightColor);

		// Renderizar las etiquetas
		shieldLabel.render(startX, startY);
		shieldValue.render(startX + 80, startY - 5);

		speedLabel.render(startX, startY + lineHeight);
		speedValue.render(startX + 80, startY + lineHeight - 5);

		atkSpeedLabel.render(startX, startY + 2 * lineHeight);
		atkSpeedValue.render(startX + 80, startY + 2 * lineHeight - 5);

		damageLabel.render(startX, startY + 3 * lineHeight);
		damageValue.render(startX + 80, startY + 3 * lineHeight - 5);
	}
}

void RunningState::leave()
{
	Mix_HaltMusic();
}
