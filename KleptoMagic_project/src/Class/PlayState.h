//#pragma once
//
//#include "GameState.h"
//#include "list"
//#include "SceneObject.h"
//#include "Collisions.h"
//#include "Player.h"
//#include "Game.h"
//
//
//class PlayState : public GameState
//{
//	//list with all the scene objects
//	std::list<SceneObject*> sceneObjects;
//	//Player
//	Player* player;
//	//Current room
//	int currentRoom;
//	//Coins
//	int coins;
//	//Timer
//	int timer;
//
//public:
//
//	//Constructor
//	PlayState(Game* game);
//
//	//Add object to the list
//	void addObject(SceneObject* object);
//
//	//Getter for Player
//	Player* getPlayer();
//
//	//Checks all the collisions
//	bool CheckCollision();
//
//};