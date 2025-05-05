#include "PlayerUtils.h"

#include "../Class/Transform.h"
#include "../Class/ImageWithFrames.h"
#include "../Class/EntityStat.h"
#include "../Class/PlayerCtrl.h"
#include "../Class/MoveThroughRooms.h"
#include "../Class/PlayerAnimComponent.h"
#include "../Class/WeaponImage.h"

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

void PlayerUtils::createPlayer(Vector2D pos, float size, BulletUtils* bullet) {
	auto player = _mngr->addEntity();
	_mngr->setHandler(ecs::hdlr::PLAYER, player);
	auto tr = _mngr->addComponent<Transform>(player);
	tr->init(pos, Vector2D(), size, size, 0.0f);
	int charStartFrame;
	if (_character == CHAR_KNIGHT) charStartFrame = 0;
	else if (_character == CHAR_HUNTER) charStartFrame = 6;
	else if (_character == CHAR_ROGUE) charStartFrame = 12;
	else if (_character == CHAR_ALCHEMIST) charStartFrame = 18;
	_mngr->addComponent<ImageWithFrames>(player, &sdlutils().images().at("player_sprites"), (float)75, 6, 4, charStartFrame, 1);
	_mngr->addComponent<EntityStat>(player, 3, 1, 10, 1, 1);
	_mngr->addComponent<PlayerCtrl>(player);
	_mngr->addComponent<WeaponImage>(player, &sdlutils().images().at("weapon_sprites"), 32.0f, int(_character));
	auto tilechecker = game().getMngr()->addComponent<TileCollisionChecker>(player);
	tilechecker->init(false, tr, _dungeonfloor);
	tr->initTileChecker(tilechecker);
	auto movethroughrooms = game().getMngr()->addComponent<MoveThroughRooms>(player);
	game().getMngr()->addComponent<PlayerAnimComponent>(player, charStartFrame);
	movethroughrooms->init(_dungeonfloor, bullet);
	movethroughrooms->enterRoom(' ');
}

void PlayerUtils::destroyPlayer() {
	_mngr->setAlive(_mngr->getHandler(ecs::hdlr::PLAYER), false);
}