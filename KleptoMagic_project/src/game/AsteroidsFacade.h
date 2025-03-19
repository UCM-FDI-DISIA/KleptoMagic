#pragma once

#include <SDL.h>
#include "../../../src/ecs/Entity.h"
#include "../../../src/ecs/Manager.h"

using namespace ecs;

class AsteroidsFacade {
public:
	AsteroidsFacade() {}
	virtual ~AsteroidsFacade() {}
	virtual void create_asteroids(int n) = 0;
	virtual void remove_all_asteroids() = 0;
	virtual void split_asteroid(Entity* a) = 0;
};