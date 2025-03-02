#include "GameState.h"
#include "GameObject.h"


void GameState::update() {
	for (GameObject* elem : gameObj) {
		elem->update();
	}
}

void GameState::render() const{
	for (GameObject* elem : gameObj) {
		elem->render();
	}

}

void GameState::handleEvent(SDL_Event e) {
	for (EventHandler* elem : eventObj) {
		elem->handleEvent(e);
	}
}


