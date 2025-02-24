#pragma once

#include "GameState.h"
#include "list"
#include "SceneObject.h"

class PlayState : public GameState
{
	//list with all the scene objects
	list<SceneObject*> sceneObjects;
	//Player
	// 
	//Current room
	// 
	//Coins
	int coins;
	//Timer
	int timer;

public:

	//bool CheckCollisions(Rect);

	//Player* GetPlayer();
};