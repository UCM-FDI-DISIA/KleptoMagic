#pragma once
#include <SDL.h>
#include "../ecs/Component.h"
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
#include "Transform.h"
#include "ImageWithFrames.h"
#include "AnimatorComponent.h"

class PlayerAnimComponent : public AnimatorComponent {
	friend AnimatorComponent;
public:
	__CMPID_DECL__(ecs::cmp::PLAYERANIMCMP);

	PlayerAnimComponent();

	void update() override;
	void toggleWalkingAnim();
	void playDeath() override;

private:
	void createStart();
};
