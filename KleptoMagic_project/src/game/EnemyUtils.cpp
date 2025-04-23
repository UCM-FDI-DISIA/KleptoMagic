#include "EnemyUtils.h"

#include "../Class/Transform.h"
#include "../Class/Image.h"
#include "../Class/TileCollisionChecker.h"

#include "../Class/SlimeComponents.h"
#include "../Class/UndeadArcherCMPS.h"
#include "../Class//LivingArmorCMP.h"
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
	case ENEMY_BOSS:
		spawn_BOSS(pos);
		break;
	}
}

void EnemyUtils::spawn_SLIME(Vector2D pos) {
	auto slime = _mngr->addEntity(ecs::grp::ENEMY);
	auto s = 50.0f;
	auto tr = _mngr->addComponent<Transform>(slime);
	tr->init(pos, Vector2D(), s, s, 0.0f);
	_mngr->addComponent<Image>(slime, &sdlutils().images().at("pacman"));
	_mngr->addComponent<SlimeVectorComponent>(slime);
	_mngr->addComponent<StatComponent>(slime);
	_mngr->addComponent<SlimeAttackComponent>(slime);
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
	_mngr->addComponent<StatComponent>(archer);
	_mngr->addComponent<UndeadVectorComponent>(archer);
	_mngr->addComponent<UndeadMovementComponent>(archer);
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
	_mngr->addComponent<Image>(armor, &sdlutils().images().at("bifrutas"));
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
	_mngr->addComponent<Image>(boss, &sdlutils().images().at("boss"));
	_mngr->addComponent<BossVectorComponent>(boss);
	_mngr->addComponent<BossStatComponent>(boss);
	_mngr->addComponent<BossAttackComponent>(boss);
	_mngr->addComponent<BossMovementComponent>(boss);
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
	_mngr->addComponent<NecroMovementComponent>(necro);
	_mngr->addComponent<NecroStatComponent>(necro);
	_mngr->addComponent<NecroSpawnerComponent>(necro);
	auto tilechecker = _mngr->addComponent<TileCollisionChecker>(necro);
	tilechecker->init(false, tr, _dungeonfloor);
	tr->initTileChecker(tilechecker);


}
void EnemyUtils::spawn_SPAWN(Vector2D pos)
{
	auto slime = _mngr->addEntity(ecs::grp::ENEMY);
	auto s = 50.0f;
	auto tr = _mngr->addComponent<Transform>(slime);
	tr->init(pos, Vector2D(), s, s, 0.0f);
	_mngr->addComponent<Image>(slime, &sdlutils().images().at("pacman"));
	_mngr->addComponent<SlimeVectorComponent>(slime);
	_mngr->addComponent<StatComponent>(slime);
	_mngr->addComponent<SlimeMovementComponent>(slime);
	_mngr->addComponent<SpawnComponent>(slime);
    auto tilechecker = _mngr->addComponent<TileCollisionChecker>(slime);
	tilechecker->init(false, tr, _dungeonfloor);
	tr->initTileChecker(tilechecker);
}

void EnemyUtils::remove_all_enemies() {
	for (auto e : _mngr->getEntities(ecs::grp::ENEMY)) {
		_mngr->setAlive(e, false);
	}
}