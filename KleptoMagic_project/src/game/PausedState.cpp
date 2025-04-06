#include "PausedState.h"

#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/InputHandler.h"

PausedState::PausedState() {

	pressAnyKey = new Texture(sdlutils().renderer(), 
		"Press any key to resume the game",
		sdlutils().fonts().at("ARIAL24"), 
		build_sdlcolor(0x112233ff),
		build_sdlcolor(0xffffffff));
	
	x0 = (sdlutils().width() - pressAnyKey->width()) / 2;
	y0 = (sdlutils().height() - pressAnyKey->height()) / 2;
}
PausedState::~PausedState() {

}

void PausedState::enter() {
	sdlutils().virtualTimer().pause();
}

void PausedState::leave() {
	sdlutils().virtualTimer().resume();
}

void PausedState::update() {
	
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

		// enter RunningState when any key is down
		if (ih().keyDownEvent()) {
			//here
			game().setState(Game::RUNNING);
			exit = true;
		}

		// clear screen
		sdlutils().clearRenderer();

		// render Press Any Key
		pressAnyKey->render(x0, y0);

		// present new frame
		sdlutils().presentRenderer();

		Uint32 frameTime = sdlutils().currRealTime() - startTime;

		if (frameTime < 20)
			SDL_Delay(20 - frameTime);
	}
}