#pragma once

#include "GameState.h"
#include "list"
#include "GameObject.h"

class DummyState : public GameState
{
private:
	std::list <GameObject*> gameObjects;
public:
	DummyState();
	void update();

};