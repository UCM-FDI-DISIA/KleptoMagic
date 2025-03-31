#pragma once
#include "Transform.h"
#include "../ecs/Component.h"
#include "../ecs/Manager.h"
#include "Game.h"
class DestroyOnBorder: public ecs::Component
{
public:
	__CMPID_DECL__(ecs::cmp::DESTROYONBORDER);
	DestroyOnBorder();
	~DestroyOnBorder();
	void initComponent();
	void update();
private:
	Transform* _tr;
};

