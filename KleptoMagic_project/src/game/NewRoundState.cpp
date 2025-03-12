#include "NewRoundState.h"

#include "FighterUtils.h"
#include "AsteroidsUtils.h"
#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/InputHandler.h"

NewRoundState::NewRoundState() {

	pressEnter = new Texture(sdlutils().renderer(), 
		"Press ENTER to start the round",
		sdlutils().fonts().at("ARIAL24"), 
		build_sdlcolor(0x112233ff),
		build_sdlcolor(0xffffffff));

	x0 = (sdlutils().width() - pressEnter->width()) / 2;
	y0 = (sdlutils().height() - pressEnter->height()) / 2;
}
NewRoundState::~NewRoundState() {

}

void NewRoundState::update() {
	
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
			game().setState(Game::RUNNING);
			exit = true;
		}

		// clear screen
		sdlutils().clearRenderer();

		// render Press Any Key
		pressEnter->render(x0, y0);

		// present new frame
		sdlutils().presentRenderer();

		Uint32 frameTime = sdlutils().currRealTime() - startTime;

		if (frameTime < 20)
			SDL_Delay(20 - frameTime);
	}
}

void NewRoundState::enter()
{
	fighterutils().reset_fighter();
	asteroidsutils().remove_all_asteroids();
	asteroidsutils().create_asteroids(10); // AJUSTE: Número de asteroides
}

void NewRoundState::leave()
{
}
