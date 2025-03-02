#pragma once

#include "GameState.h"
#include "list"
#include "GameObject.h"

class DummyState : public GameState
{
private:
	Game* game;
	std::list <GameObject*> gameObjects;
public:
	DummyState();
	void update();

};