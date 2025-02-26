#pragma once

#include "GameState.h"
#include "list"
#include "GameObject.h"

class DummyState
{
private:
	std::list <GameObject*> gameObjects;
public:
	DummyState();
	void update();

};

