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

	PlayerAnimComponent(int startF);

	void initComponent() override;
	void update();
	void toggleWalkingAnim();
	void toggleFlip();

private:
	Transform* _tr;
	ImageWithFrames* _plrImg;
	int startFrame;
	int deathFrame;
	bool isWalking;
	bool isFacingRight;
	void createStart();
};
