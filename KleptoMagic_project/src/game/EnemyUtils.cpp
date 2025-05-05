#include "EnemyUtils.h"

#include "../Class/Transform.h"
#include "../Class/Image.h"
#include "../Class/ImageWithFrames.h"

#include "../Class/TileCollisionChecker.h"

#include "../Class/SlimeComponents.h"
#include "../Class/UndeadArcherCMPS.h"
#include "../Class/LivingArmorCMP.h"
#include "../Class/GhostComponent.h"
#include "../Class/BossCMP.h"
#include "../Class/NecromancerComponent.h"

#include "../sdlutils/SDLUtils.h"

#include <iostream>
using namespace std;

EnemyUtils::EnemyUtils() :
	_mngr(nullptr), _dungeonfloor(nullptr)
{
}
EnemyUtils::~EnemyUtils() {
	remove_all_enemies();
}

bool EnemyUtils::init(Manager* mngr) {
	_mngr = mngr;
	assert(_mngr != nullptr);
	return true;
}

void EnemyUtils::spawn_enemy(EnemyNames name, Vector2D pos) {
	switch (name) {
	case ENEMY_SLIME:
		spawn_SLIME(pos);
		break;
	case ENEMY_ARCHER:
		spawn_ARCHER(pos);
		break;
	case ENEMY_ARMOR:
		spawn_ARMOR(pos);
		break;
	case ENEMY_GHOST:
		spawn_GHOST(pos);
		break;
	case ENEMY_NECRO:
		spawn_NECRO(pos);
		break;
	case ENEMY_BOSS:
		spawn_BOSS(pos);
		break;
	case ENEMY_CHEST:
		spawn_CHEST(pos); 
		break;
	//case ENEMY_SPAWN:
		//spawn_SPAWN(pos);
		//break;
	}
}

void EnemyUtils::spawn_SLIME(Vector2D pos) {
	auto slime = _mngr->addEntity(ecs::grp::ENEMY);
	auto s = 50.0f;
	auto tr = _mngr->addComponent<Transform>(slime);
	tr->init(pos, Vector2D(), s, s, 0.0f);
	//_mngr->addComponent<Image>(slime, &sdlutils().images().at("pacman"));
	_mngr->addComponent<ImageWithFrames>(slime, &sdlutils().images().at("slime_sprites"), 4, 2, 1);
	_mngr->addComponent<SlimeVectorComponent>(slime);
	_mngr->addComponent<SlimeStatComponent>(slime);
	_mngr->addComponent<SlimeMovementComponent>(slime);

	auto tilechecker = _mngr->addComponent<TileCollisionChecker>(slime);
	tilechecker->init(false, tr, _dungeonfloor);
	tr->initTileChecker(tilechecker);
}

void EnemyUtils::spawn_ARCHER(Vector2D pos) {
	auto archer = _mngr->addEntity(ecs::grp::ENEMY);
	auto s = 50.0f;
	auto tr = _mngr->addComponent<Transform>(archer);
	tr->init(pos, Vector2D(), s, s, 0.0f);
	_mngr->addComponent<Image>(archer, &sdlutils().images().at("star"));
	_mngr->addComponent<UndeadStatComponent>(archer);
	_mngr->addComponent<UndeadVectorComponent>(archer);
	auto mArcher = _mngr->addComponent<UndeadMovementComponent>(archer);
	mArcher->init(_dungeonfloor);
	_mngr->addComponent<UndeadAttackComponent>(archer);

	auto tilechecker = _mngr->addComponent<TileCollisionChecker>(archer);
	tilechecker->init(false, tr, _dungeonfloor);
	tr->initTileChecker(tilechecker);
}

void EnemyUtils::spawn_ARMOR(Vector2D pos) {
	auto armor = _mngr->addEntity(ecs::grp::ENEMY);
	auto s = 50.0f;
	auto tr = _mngr->addComponent<Transform>(armor);
	tr->init(pos, Vector2D(), s, s, 0.0f);
	//_mngr->addComponent<Image>(armor, &sdlutils().images().at("bifrutas"));
	_mngr->addComponent<ImageWithFrames>(armor, &sdlutils().images().at("armor_sprites"), 4, 5, 1);
	_mngr->addComponent<ArmorVectorComponent>(armor);
	_mngr->addComponent<ArmorStatComponent>(armor);
	_mngr->addComponent<ArmorAttackComponent>(armor);
	_mngr->addComponent<ArmorMovementComponent>(armor);
	auto tilechecker = _mngr->addComponent<TileCollisionChecker>(armor);
	tilechecker->init(false, tr, _dungeonfloor);
	tr->initTileChecker(tilechecker);
}

void EnemyUtils::spawn_GHOST(Vector2D pos) {
	auto ghost = _mngr->addEntity(ecs::grp::ENEMY);
	auto s = 50.0f;
	auto tr = _mngr->addComponent<Transform>(ghost);
	tr->init(pos, Vector2D(), s, s, 0.0f);
	_mngr->addComponent<Image>(ghost, &sdlutils().images().at("bifrutas"));
	_mngr->addComponent<GhostComponent>(ghost);
}

void EnemyUtils::spawn_BOSS(Vector2D pos) {
	auto boss = _mngr->addEntity(ecs::grp::ENEMY);
	auto s = 50.0f;
	auto tr = _mngr->addComponent<Transform>(boss);
	tr->init(pos, Vector2D(), s, s, 0.0f);
	_mngr->addComponent<Image>(boss, &sdlutils().images().at("ALCHEMIST"));
	_mngr->addComponent<BossVectorComponent>(boss);
	_mngr->addComponent<BossStatComponent>(boss);
	_mngr->addComponent<BossAttackComponent>(boss);
	auto mBoss = _mngr->addComponent<BossMovementComponent>(boss);
	mBoss->init(_dungeonfloor);
	auto tilechecker = _mngr->addComponent<TileCollisionChecker>(boss);
	tilechecker->init(false, tr, _dungeonfloor);
	tr->initTileChecker(tilechecker);
}
void EnemyUtils::spawn_NECRO(Vector2D pos) 
{
	auto necro = _mngr->addEntity(ecs::grp::ENEMY);
	auto s = 50.0f;
	auto tr = _mngr->addComponent<Transform>(necro);
	tr->init(pos, Vector2D(), s, s, 0.0f);
	_mngr->addComponent<Image>(necro, &sdlutils().images().at("bifrutas"));
	_mngr->addComponent<NecroVectorComponent>(necro);
	_mngr->addComponent<NecroStatComponent>(necro);
	_mngr->addComponent<NecroSpawnerComponent>(necro);
	auto tilechecker = _mngr->addComponent<TileCollisionChecker>(necro);
	tilechecker->init(false, tr, _dungeonfloor);
	tr->initTileChecker(tilechecker);


}
void EnemyUtils::necro_spawn(Entity* necro, int x, int y) 
{
	auto slime = _mngr->addEntity(ecs::grp::ENEMY);
	auto s = 50.0f;
	auto tr = _mngr->addComponent<Transform>(slime);
	Vector2D pos;
	auto _necrotr = _mngr->getComponent<Transform>(necro);
	int resultX = _dungeonfloor->checkCollisions(_necrotr->getPos().getX() + x , _necrotr->getPos().getY() + y);
	if (resultX == 0 ) 
	{
		pos = { _necrotr->getPos().getX() + x , _necrotr->getPos().getY() + y };
	}
	else 
	{
		pos = { _necrotr->getPos().getX() , _necrotr->getPos().getY()};
	}
	
	tr->init(pos, Vector2D(), s, s, 0.0f);
	_mngr->addComponent<Image>(slime, &sdlutils().images().at("pacman"));
	_mngr->addComponent<SlimeVectorComponent>(slime);
	_mngr->addComponent<SpawnedStatComponent>(slime);
	_mngr->addComponent<SpawnMovementComponent>(slime);
	auto spawned = _mngr->addComponent<SpawnComponent>(slime);
	spawned->setParent(necro);
	auto tilechecker = _mngr->addComponent<TileCollisionChecker>(slime);
	tilechecker->init(false, tr, _dungeonfloor);
	tr->initTileChecker(tilechecker);
}
/*void EnemyUtils::spawn_SPAWN(Vector2D pos)
{
	auto slime = _mngr->addEntity(ecs::grp::ENEMY);
	auto s = 50.0f;
	auto tr = _mngr->addComponent<Transform>(slime);
	tr->init(pos, Vector2D(), s, s, 0.0f);
	_mngr->addComponent<Image>(slime, &sdlutils().images().at("pacman"));
	_mngr->addComponent<SlimeVectorComponent>(slime);
	_mngr->addComponent<SlimesStatComponent>(slime);
	_mngr->addComponent<SlimeMovementComponent>(slime);
	_mngr->addComponent<SpawnComponent>(slime);
    auto tilechecker = _mngr->addComponent<TileCollisionChecker>(slime);
	tilechecker->init(false, tr, _dungeonfloor);
	tr->initTileChecker(tilechecker);
}*/

void EnemyUtils::spawn_CHEST(Vector2D pos) {
	auto chest = _mngr->addEntity(ecs::grp::ENEMY);							// Treat chest like an enemy
	auto s = 50.0f;
	auto tr = _mngr->addComponent<Transform>(chest);
	tr->init(pos, Vector2D(), s, s, 0.0f);
	//_mngr->addComponent<Image>(chest, &sdlutils().images().at("chest"));	// Add a chest sprite
	//_mngr->addComponent<RewardsComponent>(chest);							// Handles loot
	//_mngr->addComponent<MinigameGeneratorComponent>(chest);				// Generates minigame on interaction
	auto tilechecker = _mngr->addComponent<TileCollisionChecker>(chest);
	tilechecker->init(false, tr, _dungeonfloor);
	tr->initTileChecker(tilechecker);
}

void EnemyUtils::remove_all_enemies() {
	for (auto e : _mngr->getEntities(ecs::grp::ENEMY)) {
		_mngr->setAlive(e, false);
	}
}