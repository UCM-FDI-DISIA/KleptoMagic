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
	//Destructor
	~PlayState();

	//Add object to the list
	void addObject(SceneObject* object);

	bool CheckCollision(SceneObject* object1, SceneObject* object2);
	//Player* GetPlayer(){return Player*;};
};