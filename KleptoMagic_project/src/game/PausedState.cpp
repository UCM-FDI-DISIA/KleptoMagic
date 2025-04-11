#include "PausedState.h"

#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/NewInputHandler.h"

PausedState::PausedState() {
#ifdef _DEBUG
	std::cout << "Nuevo PauseState creado!" << std::endl;
#endif
}
PausedState::~PausedState() {

}

void PausedState::enter() {
	sdlutils().virtualTimer().pause();
	// Cargar el fondo
	background = new Texture(sdlutils().renderer(), "resources/images/pausemenu-provisional.png");

	// Provisional
	/*pressAnyKey = new Texture(sdlutils().renderer(),
		"Press any key to resume the game",
		sdlutils().fonts().at("ARIAL24"),
		build_sdlcolor(0x112233ff),
		build_sdlcolor(0xffffffff));*/

	//x0 = (sdlutils().width() - pressAnyKey->width()) / 2;
	//y0 = (sdlutils().height() - pressAnyKey->height()) / 2;

	titule = new Texture(sdlutils().renderer(),
		"PAUSE",
		sdlutils().fonts().at("ARIAL48"),
		build_sdlcolor(0x112233ff),
		build_sdlcolor(0xffffffff));

	x1 = (sdlutils().width() - titule->width()) / 2;
	y1 = titule->height();

	// Crear el bot�n Resume
	resumeTexture = new Texture(sdlutils().renderer(), "resources/images/select.png");

	float btnWidth = resumeTexture->width() / 4;
	float btnHeight = resumeTexture->height() / 4;
	float centerX = (sdlutils().width() - btnWidth) / 2;
	float baseY = sdlutils().height() * 0.50f;
	float playBtnX = centerX - 50;
	float playBtnY = baseY;

	resumeButton = new Button([this]() {
		game().setState(Game::RUNNING);
		}, Vector2D(playBtnX, playBtnY), Vector2D(btnWidth, btnHeight), resumeTexture, "button");
}

void PausedState::leave() {
	sdlutils().virtualTimer().resume();
}

void PausedState::update() {
	
	bool exit = false;

	// reset the time before starting - so we calculate correct
	// delta-time in the first iteration
	//
	sdlutils().resetTime();

	while (!exit) {
		Uint32 startTime = sdlutils().currRealTime();

		/*while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				exit = true;
				game().quit();
			}
			resumeButton->handleEvent(event);
		}*/

		NewInputHandler::Instance()->update();

		resumeButton->update(); // Detecta si fue presionado

		if (resumeButton->isPressed()) {
			exit = true; // Salimos del estado pausado
		}

		// clear screen
		sdlutils().clearRenderer();

		// Render background picture
		background->render({ 0, 0, sdlutils().width(), sdlutils().height() });

		// render Press Any Key
		//pressAnyKey->render(x0, y0);
		titule->render(x1, y1);
		resumeButton->render();

		// present new frame
		sdlutils().presentRenderer();

		Uint32 frameTime = sdlutils().currRealTime() - startTime;

		if (frameTime < 20)
			SDL_Delay(20 - frameTime);
	}
}