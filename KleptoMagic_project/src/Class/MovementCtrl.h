#pragma once
#include "../utils/Vector2D.h"
#include "../ecs/Component.h"
#include "Transform.h"

class MovementCtrl : public ecs::Component {
public:
	__CMPID_DECL__(ecs::cmp::MOVEMENTCTRL);

	void initComponent() override; //para coger el Vector2D / transform si se cambia a eso
	void Move(Transform& transform, Vector2D movementVector, float speed);


private:
	void createStart();
	//vector2D o transform
};
