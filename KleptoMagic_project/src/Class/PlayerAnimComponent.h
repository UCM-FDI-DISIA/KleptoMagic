#pragma once
#include <SDL.h>
#include "../ecs/Component.h"
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
#include "Transform.h"
#include "ImageWithFrames.h"

class PlayerAnimComponent : public ecs::Component {
public:
	__CMPID_DECL__(ecs::cmp::PLAYERANIMCOMPONENT);

	void initComponent() override;
	void update();
	void toggleWalking() { walking = !walking; };

private:
	Transform* _tr;
	ImageWithFrames* _plrImg;
	bool walking;
	int walkingFrame;
	void createStart();
};
