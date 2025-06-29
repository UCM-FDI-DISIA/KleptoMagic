#pragma once
#include "../ecs/Component.h"
#include "../ecs/Transform.h"
#include "../room/ObjectUtils.h"

using namespace ecs;

const int DEFAULT_DROP_RATE = 20;	// % de posibilidad de que dropee

class EnemyDrop : public Component
{
private:
	Transform* _entTr;		// puntero al transform del enemigo

	string _upgradeName;	// nombre del upgrade si lo hubiese

public:
	__CMPID_DECL__(ecs::cmp::PICKABLECMP);
	
	EnemyDrop(string name);
	EnemyDrop();

	void initComponent() override;
	void playerCollision();
	void update() override;
	void spawnUpgrade();

};

