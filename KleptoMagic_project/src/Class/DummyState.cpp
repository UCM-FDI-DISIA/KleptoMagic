#include "DummyState.h"

DummyState::DummyState() : GameState(game){
	_inputManager = new InputManager();
	_inputManager->initialize();
}

void DummyState::update() {
	_inputManager->update();
}