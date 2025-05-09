#pragma once
#include <SDL.h>
#include "../ecs/Component.h"
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
#include "../ecs/Transform.h"
#include "ImageWithFrames.h"

class AnimatorComponent : public ecs::Component {
public:
	__CMPID_DECL__(ecs::cmp::ANIMATORCOMPONENT);

	AnimatorComponent() {};

	void initComponent();

	virtual void toggleFlip();
	virtual void playDeath();

	void setStartFrame(int f) { startFrame = f; };
	void setDeathFrame(int f) { deathFrame = f; };

protected:
	Transform* _tr;
	ImageWithFrames* _img;
	int startFrame;
	int deathFrame;
	bool isWalking;
	bool isFacingRight;
	void createStart();
};
