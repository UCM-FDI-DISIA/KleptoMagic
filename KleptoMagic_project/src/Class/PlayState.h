#pragma once

#include "GameState.h"
#include "list"
#include "SceneObject.h"
#include <vector>

//#include "GameManager.h"


class PlayState : public GameState
{
	//list with all the scene objects
	std::list <SceneObject*> sceneObjects;
	//Player
	// 
	//Current room
	int currentRoom;
	//Coins
	int coins;
	//Timer
	int timer;
	
public:

	//bool CheckCollisions(Rect);

	
	//Player* GetPlayer();
};