#include "GameOverState.h"

#include "AsteroidsUtils.h"
#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/InputHandler.h"

GameOverState::GameOverState() {

}
GameOverState::~GameOverState() {

}

void GameOverState::enter() {
	//if (asteroidsutils().count_asteroids() <= 0) {
		text = new Texture(sdlutils().renderer(),
			"Game Over Champion! Press ENTER to continue.",
			sdlutils().fonts().at("ARIAL24"),
			build_sdlcolor(0x112233ff),
			build_sdlcolor(0xffffffff));
	//}
	//else {
	//	text = new Texture(sdlutils().renderer(),
	//		"Game Over Loser! Press ENTER to continue.",
	//		sdlutils().fonts().at("ARIAL24"),
	//		build_sdlcolor(0x112233ff),
	//		build_sdlcolor(0xffffffff));
	////}
	//
	//x0 = (sdlutils().width() - text->width()) / 2;
	//y0 = (sdlutils().height() - text->height()) / 2;
}

void GameOverState::update() {
	
	bool exit = false;
	auto& ihdlr = ih();

	// reset the time before starting - so we calculate correct
	// delta-time in the first iteration
	//
	sdlutils().resetTime();

	while (!exit) {
		Uint32 startTime = sdlutils().currRealTime();

		// update the event handler
		ih().refresh();

		// enter NewGameState when any key is down
		if (ih().keyDownEvent()) {
			//here
			game().setState(Game::NEWGAME);
			exit = true;
		}

		// clear screen
		sdlutils().clearRenderer();

		// render text
		text->render(x0, y0);

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
