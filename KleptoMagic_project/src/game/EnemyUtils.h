#pragma once

#include "EnemyFacade.h"
#include "../../src/utils/Singleton.h"
#include "../utils/Vector2D.h"


using namespace ecs;

class EnemyUtils : virtual public EnemyFacade, public Singleton<EnemyUtils> {
	friend Singleton<EnemyUtils>;
protected:
	EnemyUtils();
	bool init(Manager* mngr);
	virtual ~EnemyUtils();
	Manager* _mngr;
	DungeonFloor* _dungeonfloor;

	void spawn_SLIME(Vector2D pos);
	void spawn_ARCHER(Vector2D pos);
	void spawn_ARMOR(Vector2D pos);
	void spawn_GHOST(Vector2D pos);
	void spawn_BOSS(Vector2D pos);
	void spawn_NECRO(Vector2D pos);
	void spawn_SPAWN(Vector2D pos);

public:
	void setDungeonFloor(DungeonFloor* floor) { _dungeonfloor = floor; }

	void spawn_enemy(EnemyNames name, Vector2D pos);
	void remove_all_enemies();
};

// This macro defines a compact way for using the singleton
inline EnemyUtils& enemyutils() {
	return *EnemyUtils::Instance();
}

