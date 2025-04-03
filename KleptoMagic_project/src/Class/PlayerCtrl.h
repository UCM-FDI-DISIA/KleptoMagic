#pragma once
#include <SDL.h>
#include "../utils/Vector2D.h"
#include "../ecs/Component.h"
#include "Transform.h"
#include "TileCollisionChecker.h"
#include "Bullet.h"
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"

class PlayerCtrl : public ecs::Component {
public:
	__CMPID_DECL__(ecs::cmp::PLAYERCTRL);

	void initComponent() override; //para coger el transform
	void Move(Vector2D movementVector, float speed); // modificar el vector de direccion y velocidad
	void update();

private:
	void createStart();
	Transform* _tr;
};
