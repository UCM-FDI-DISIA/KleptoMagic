#include "GameOverState.h"

#include "AsteroidsUtils.h"
#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/NewInputHandler.h"

GameOverState::GameOverState() {

}
GameOverState::~GameOverState() {

}

void GameOverState::enter() {

	// Cargar el fondo
	background = new Texture(sdlutils().renderer(), "resources/images/endMenu.png");

}

void GameOverState::update() {
	
	bool exit = false;

	// reset the time before starting - so we calculate correct
	// delta-time in the first iteration
	//
	sdlutils().resetTime();

	while (!exit) {
		Uint32 startTime = sdlutils().currRealTime();

		// update the event handler
		NewInputHandler::Instance()->update();

		// enter NewGameState when any key is down
		/*if (NewInputHandler::Instance()->isAnyKeyPressed()) {
			//here
			game().setState(Game::NEWGAME);
			exit = true;
		}*/

		// clear screen
		sdlutils().clearRenderer();

		// Render background picture
		background->render({ 0, 0, sdlutils().width(), sdlutils().height() });

		// present new frame
		sdlutils().presentRenderer();

		Uint32 frameTime = sdlutils().currRealTime() - startTime;

		if (frameTime < 20)
			SDL_Delay(20 - frameTime);
	}
}

void GameOverState::leave()
{
}
