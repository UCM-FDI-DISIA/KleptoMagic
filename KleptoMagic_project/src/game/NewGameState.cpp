#include "NewGameState.h"

#include "FighterUtils.h"
#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/InputHandler.h"


using ecs::Manager;	

NewGameState::NewGameState(){

	pressAnyKey = new Texture(sdlutils().renderer(), 
		"Press any key to start",
		sdlutils().fonts().at("ARIAL24"), 
		build_sdlcolor(0x112233ff),
		build_sdlcolor(0xffffffff));

	x0 = (sdlutils().width() - pressAnyKey->width()) / 2;
	y0 = (sdlutils().height() - pressAnyKey->height()) / 2;

	
	

}
NewGameState::~NewGameState() {

}

void NewGameState::update() {
	
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

		// enter NewRoundState when any key is down
		if (ih().keyDownEvent()) {
			//fighterutils().reset_lives();
			//here
			game().setState(Game::NEWROUND);
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

void NewGameState::enter()
{
	//fighterutils().reset_lives();
}

void NewGameState::leave()
{
}
