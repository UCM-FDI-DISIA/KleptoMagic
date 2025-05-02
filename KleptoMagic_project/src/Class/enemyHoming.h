#pragma once
#include "../ecs/Component.h"
#include "../Class/Game.h"
#include "../Class/Transform.h"
#include "../ecs/Manager.h"
class enemyHoming : public ecs::Component
{
public:
	__CMPID_DECL__(ecs::cmp::ENEMYHOMING);

	enemyHoming();
	~enemyHoming();
	void initComponent();
	void update();
private:
	Transform* _tr;

};