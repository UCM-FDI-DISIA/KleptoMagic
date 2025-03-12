#include "DummyState.h"

DummyState::DummyState() : GameState(game), gameTimer(300) {
	_inputManager = new InputManager();
	_inputManager->initialize();
}

void DummyState::update() {
	_inputManager->update();
}