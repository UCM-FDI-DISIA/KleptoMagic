////#include "checkML.h"
////#include "gameList.h"
//
////#include "SceneObject.h"
//#include "EventHandler.h"
//#include "InputManager.h"
//
//#include <list>
//
//class GameObject;
//class Game;
//
//#pragma once
//class GameState_OLD
//{
//protected:
//	Game* game;
//	//GameList<GameObject> gameObj;
//	std::list<GameObject*> gameObj;
//	std::list<EventHandler*> eventObj;
//
//
//public:
//	GameState_OLD(Game* game) : game(game) {}
//	virtual void update();
//	virtual void render() const;
//	virtual void handleEvent(SDL_Event e);
//	
//	void addEventListener(EventHandler* eventLis) {
//		eventObj.push_back(eventLis);
//	}
//	
//	void addObject(GameObject* obj) {
//		gameObj.push_back(obj);
//	}
//	//void map_reload();
//
//	Game* getGame() const { return game; }
//};
//
//