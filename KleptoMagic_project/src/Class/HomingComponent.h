#pragma once
#include "../ecs/Component.h"
#include "../Class/Game.h"
#include "../Class/Transform.h"
#include "../ecs/Manager.h"
class HomingComponent: public ecs::Component
{
public:
	__CMPID_DECL__(ecs::cmp::HOMINGCOMPONENT);

	HomingComponent();
	~HomingComponent();
	void initComponent();
void update();
private:
	Transform* _tr;
};