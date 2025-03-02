#pragma once

#include "GameState.h"
#include "list"
#include "GameObject.h"
#include "TimerCountdown.h"

class DummyState : public GameState
{
private:
	Game* game;
	std::list <GameObject*> gameObjects;
	TimerCountdown gameTimer;
	bool eventActive;

public:
	DummyState();
	void update();
	void timerTest();
};