#pragma once

#include "GameState.h"
#include "list"
#include "GameObject.h"
#include "InputManager.h"

class DummyState : public GameState
{
private:
	Game* game;
	std::list <GameObject*> gameObjects;
	InputManager* _inputManager;
public:
	DummyState();
	void update();
};