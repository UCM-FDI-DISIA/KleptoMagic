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
	background = new Texture(sdlutils().renderer(), "resources/images/pauseMenu.png");

	// Cargar imagen de titulo
	title = new Texture(sdlutils().renderer(), "resources/images/pause.png");

	// Escalado al tamaño del boton
	titleWidth = title->width() / 2;
	titleHeight = title->height() / 2;

	// Centrado horizontal y arriba con margen
	x0 = (sdlutils().width() - titleWidth) / 2;
	y0 = sdlutils().height() * 0.2f - 120; 

	// Crear el boton Resume
	resumeTexture = new Texture(sdlutils().renderer(), "resources/images/resume.png");

	float btnWidth = resumeTexture->width() / 4;
	float btnHeight = resumeTexture->height() / 4;

	float playBtnX = (sdlutils().width() - btnWidth) / 2;
	float playBtnY = (sdlutils().height() - btnHeight) / 2 + 100;

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

		SDL_Rect dest = { static_cast<int>(x0), static_cast<int>(y0), static_cast<int>(titleWidth), static_cast<int>(titleHeight) };
		title->render(dest);
		resumeButton->render();

		// present new frame
		sdlutils().presentRenderer();

		Uint32 frameTime = sdlutils().currRealTime() - startTime;

		if (frameTime < 20)
			SDL_Delay(20 - frameTime);
	}
}