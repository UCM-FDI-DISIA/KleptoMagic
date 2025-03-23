#pragma once

#include <SDL.h>
#include "../../../src/ecs/Entity.h"
#include "../../../src/ecs/Manager.h"

using namespace ecs;

class FighterFacade {
public:
	FighterFacade() {}
	virtual ~FighterFacade() {}
	void virtual create_fighter() = 0;
	void virtual reset_fighter() = 0;
	void virtual reset_lives() = 0;
	void virtual update_lives(int n) = 0;
};