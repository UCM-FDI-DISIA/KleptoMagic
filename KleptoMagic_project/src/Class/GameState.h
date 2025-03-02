//#include "checkML.h"
//#include "gameList.h"

//#include "SceneObject.h"
//#include "EventHandler.h"

#include <list>

class GameObject;
class Game;

#pragma once
class GameState
{
protected:
	Game* game;
	//GameList<GameObject> gameObj;
	std::list<GameObject*> gameObj;
	//list<EventHandler*> eventObj;


public:
	GameState(Game* game) : game(game) {}
	virtual void update();
	virtual void render() const;
	//virtual void handleEvent(SDL_Event e);
	//
	//void addEventListener(EventHandler* eventLis) {
	//	eventObj.push_back(eventLis);
	//}
	//
	void addObject(GameObject* obj) {
		gameObj.push_back(obj);
	}
	//void map_reload();

	Game* getGame() const { return game; }
};

