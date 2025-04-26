#include "PlayerUtils.h"

PlayerUtils::PlayerUtils() :
	_mngr(nullptr), _dungeonfloor(nullptr)
{
	_character = CHAR_KNIGHT;
}

PlayerUtils::~PlayerUtils() {
	destroyPlayer();
}

bool PlayerUtils::init(Manager* mngr) {
	_mngr = mngr;
	return true;
}

void PlayerUtils::createPlayer(Vector2D pos, float size) {
	auto player = game().getMngr()->addEntity();
	game().getMngr()->setHandler(ecs::hdlr::PLAYER, player);
	auto tr = game().getMngr()->addComponent<Transform>(player);
	tr->init(pos, Vector2D(), size, size, 0.0f);
	int charStartFrame;
	if (_character == CHAR_KNIGHT) charStartFrame = 0;
	else if (_character == CHAR_HUNTER) charStartFrame = 6;
	else if (_character == CHAR_ROGUE) charStartFrame = 12;
	else if (_character == CHAR_ALCHEMIST) charStartFrame = 18;
	game().getMngr()->addComponent<ImageWithFrames>(player, &sdlutils().images().at("player_sprites"), (float)75, 6, 4, charStartFrame, 1);
	game().getMngr()->addComponent<EntityStat>(player, 3, 1, 10, 1, 1);
	game().getMngr()->addComponent<PlayerCtrl>(player);
	auto tilechecker = game().getMngr()->addComponent<TileCollisionChecker>(player);
	tilechecker->init(false, tr, dungeonfloor);
	tr->initTileChecker(tilechecker);
	auto movethroughrooms = game().getMngr()->addComponent<MoveThroughRooms>(player);
	game().getMngr()->addComponent<PlayerAnimComponent>(player, charStartFrame);
	bullet = new BulletUtils();
	bullet->setDungeonFloor(dungeonfloor);
	movethroughrooms->init(dungeonfloor, bullet);
	movethroughrooms->enterRoom(' ');
}

void PlayerUtils::destroyPlayer() {
	_mngr->setAlive(_mngr->getHandler(ecs::hdlr::PLAYER), false);
}