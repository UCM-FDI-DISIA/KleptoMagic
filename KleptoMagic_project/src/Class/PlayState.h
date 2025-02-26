#pragma once

#include "GameState.h"
#include "list"
#include "SceneObject.h"
#include "Collisions.h"
//#include "Player.h"


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
	//Constructor
	PlayState();

	//Add object to the list
	void addObject(SceneObject* object);

	//Checks all the collisions
	bool CheckCollision();
	//Getter for Player
	//Player* getPlayer();
};