#include "NewRoundState.h"

#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/NewInputHandler.h"

NewRoundState::NewRoundState() {

	// Cargar el fondo
	background = new Texture(sdlutils().renderer(), "resources/images/selectmenu-provisional.png");

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

	// reset the time before starting - so we calculate correct
	// delta-time in the first iteration
	//
	sdlutils().resetTime();

	while (!exit) {
		Uint32 startTime = sdlutils().currRealTime();

		// update the event handler
		NewInputHandler::Instance()->update();

		// enter RunningState when any key is down
		if (NewInputHandler::Instance()->isAnyKeyPressed()) {
			game().setState(Game::RUNNING);
			exit = true;
		}

		// clear screen
		sdlutils().clearRenderer();

		// Dibujar el fondo
		SDL_Rect destRect = { 0, 0, sdlutils().width(), sdlutils().height() };
		background->render(destRect);

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
#ifdef _DEBUG
	std::cout << "Entrando en NewRoundState" << std::endl;
#endif
}

void NewRoundState::leave()
{
}
