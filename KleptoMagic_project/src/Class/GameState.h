//#include "checkML.h"
//#include "gameList.h"
//#include "GameObject.h"
//#include "SceneObject.h"
//#include "EventHandler.h"

#include <list>


#pragma once
class GameState
{
private:
	//Game* game;
	//GameList<GameObject> gameObj;
	//list<EventHandler*> eventObj;


public:
	virtual void update();
	virtual void render();
	//virtual void handleEvent(SDL_Event e);
	//
	//void addEventListener(EventHandler* eventLis) {
	//	eventObj.push_back(eventLis);
	//}
	//
	//void addObject(GameObject* obj) {
	//	gameObj.push_back(obj);
	//}
	//void map_reload();

};

