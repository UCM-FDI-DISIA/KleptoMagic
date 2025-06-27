#include "PlayerUtils.h"

#include "../ecs/Transform.h"
#include "../render/ImageWithFrames.h"
#include "../ecs/EntityStat.h"
#include "../player/PlayerCtrl.h"
#include "../room/MoveThroughRooms.h"
#include "../player/PlayerAnimComponent.h"
#include "../render/WeaponImage.h"

PlayerUtils::PlayerUtils() :
	_mngr(nullptr), _dungeonfloor(nullptr),
	knightAttackSound(nullptr), rogueAttackSound(nullptr),
	hunterAttackSound(nullptr), alchemistAttackSound(nullptr)
{
	_character = CHAR_KNIGHT;
	loadSounds();
}

PlayerUtils::~PlayerUtils() {
	destroyPlayer();
	freeSounds();
}

void PlayerUtils::loadSounds() {
	knightAttackSound = Mix_LoadWAV("resources/sound/knightAttack.mp3");
	rogueAttackSound = Mix_LoadWAV("resources/sound/rogueAttack.mp3");
	hunterAttackSound = Mix_LoadWAV("resources/sound/hunterButton.wav");
	alchemistAttackSound = Mix_LoadWAV("resources/sound/alchemistAttack.mp3");

	if (!knightAttackSound || !rogueAttackSound || !hunterAttackSound || !alchemistAttackSound) {
#ifdef _DEBUG
		std::cerr << "Error cargando sonidos de ataque: " << Mix_GetError() << std::endl;
#endif
	}

	Mix_VolumeChunk(knightAttackSound, 64);
	Mix_VolumeChunk(rogueAttackSound, 64);
	Mix_VolumeChunk(hunterAttackSound, 64);
	Mix_VolumeChunk(alchemistAttackSound, 64);
}

void PlayerUtils::freeSounds() {
	if (knightAttackSound) Mix_FreeChunk(knightAttackSound);
	if (rogueAttackSound) Mix_FreeChunk(rogueAttackSound);
	if (hunterAttackSound) Mix_FreeChunk(hunterAttackSound);
	if (alchemistAttackSound) Mix_FreeChunk(alchemistAttackSound);

	knightAttackSound = nullptr;
	rogueAttackSound = nullptr;
	hunterAttackSound = nullptr;
	alchemistAttackSound = nullptr;
}

void PlayerUtils::playAttackSound() {
	Mix_Chunk* soundToPlay = nullptr;

	switch (_character) {
	case CHAR_KNIGHT:
		soundToPlay = knightAttackSound;
		break;
	case CHAR_ROGUE:
		soundToPlay = rogueAttackSound;
		break;
	case CHAR_HUNTER:
		soundToPlay = hunterAttackSound;
		break;
	case CHAR_ALCHEMIST:
		soundToPlay = alchemistAttackSound;
		break;
	}

	if (soundToPlay) {
		Mix_PlayChannel(-1, soundToPlay, 0);
	}
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
	_mngr->addComponent<EntityStat>(player, 8, 1, 10, 1, 1);
	_mngr->addComponent<PlayerCtrl>(player);
	_mngr->addComponent<WeaponImage>(player, &sdlutils().images().at("weapon_sprites"), 32.0f, int(_character));
	auto tilechecker = game().getMngr()->addComponent<TileCollisionChecker>(player);
	tilechecker->init(false, tr, _dungeonfloor);
	tr->initTileChecker(tilechecker);
	auto movethroughrooms = game().getMngr()->addComponent<MoveThroughRooms>(player);
	auto animator = game().getMngr()->addComponent<PlayerAnimComponent>(player);
	animator->setStartFrame(charStartFrame);
	animator->setDeathFrame(charStartFrame + 4);
	movethroughrooms->init(_dungeonfloor, bullet);
	movethroughrooms->enterRoom(' ');
}

void PlayerUtils::destroyPlayer() {
	_mngr->setAlive(_mngr->getHandler(ecs::hdlr::PLAYER), false);
}