#include "NewRoundState.h"

#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/NewInputHandler.h"

#include "NewRoundState.h"
#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/InputHandler.h"

NewRoundState::NewRoundState() : selectedCharacter("") {
#ifdef _DEBUG
	std::cout << "Nuevo NewRoundState creado!" << std::endl;
#endif

	/*background = new Texture(sdlutils().renderer(), "resources/images/selectmenu-provisional.png");

	float btnWidth = 100; // Ancho del bot�n
	float btnHeight = 100; // Alto del bot�n
	float spacing = 50; // Espacio entre botones
	float startX = (sdlutils().width() - (btnWidth * 4 + spacing * 3)) / 2;
	float btnY = sdlutils().height() / 2 - btnHeight / 2;

	rogueButton = new Button([this]() {
		std::cout << "Seleccionado: ROGUE" << std::endl; // Depuraci�n
		selectedCharacter = "ROGUE";
		game().setSelectedCharacter(selectedCharacter);
		std::cout << "getSelectedCharacter: " << game().getSelectedCharacter() << std::endl;
		game().setState(Game::RUNNING);
		}, Vector2D(startX, btnY), Vector2D(btnWidth, btnHeight), &sdlutils().images().at("ROGUE"));

	knightButton = new Button([this]() {
		selectedCharacter = "KNIGHT";
		game().setSelectedCharacter(selectedCharacter);
		game().setState(Game::RUNNING);
		}, Vector2D(startX + (btnWidth + spacing), btnY), Vector2D(btnWidth, btnHeight), &sdlutils().images().at("KNIGHT"));

	alchemistButton = new Button([this]() {
		selectedCharacter = "ALCHEMIST";
		game().setSelectedCharacter(selectedCharacter);
		game().setState(Game::RUNNING);
		}, Vector2D(startX + 2 * (btnWidth + spacing), btnY), Vector2D(btnWidth, btnHeight), &sdlutils().images().at("ALCHEMIST"));

	hunterButton = new Button([this]() {
		selectedCharacter = "HUNTER";
		game().setSelectedCharacter(selectedCharacter);
		game().setState(Game::RUNNING);
		}, Vector2D(startX + 3 * (btnWidth + spacing), btnY), Vector2D(btnWidth, btnHeight), &sdlutils().images().at("HUNTER"));*/
}

void NewRoundState::update() {
	bool exit = false;
	//auto& ihdlr = ih();

	sdlutils().resetTime();

	while (!exit) {
		Uint32 startTime = sdlutils().currRealTime();
		//ih().refresh();
    NewInputHandler::Instance()->update();

		rogueButton->update();
		knightButton->update();
		alchemistButton->update();
		hunterButton->update();

		sdlutils().clearRenderer();
		background->render({ 0, 0, sdlutils().width(), sdlutils().height() });

		rogueButton->render();
		knightButton->render();
		alchemistButton->render();
		hunterButton->render();

		sdlutils().presentRenderer();

		if (!selectedCharacter.empty()) {
			exit = true;
		}

		Uint32 frameTime = sdlutils().currRealTime() - startTime;
		if (frameTime < 20)
			SDL_Delay(20 - frameTime);
	}
}
NewRoundState::~NewRoundState() {

}

void NewRoundState::enter()
{
#ifdef _DEBUG
	std::cout << "Entrando en NewRoundState" << std::endl;
#endif

	background = new Texture(sdlutils().renderer(), "resources/images/selectmenu-provisional.png");

	float btnWidth = 100; // Ancho del bot�n
	float btnHeight = 100; // Alto del bot�n
	float spacing = 50; // Espacio entre botones
	float startX = (sdlutils().width() - (btnWidth * 4 + spacing * 3)) / 2;
	float btnY = sdlutils().height() / 2 - btnHeight / 2;

	rogueButton = new Button([this]() {
#ifdef _DEBUG
		std::cout << "Seleccionado: ROGUE" << std::endl; // Depuraci�n
#endif
		selectedCharacter = "ROGUE";
		game().setSelectedCharacter(selectedCharacter);
#ifdef _DEBUG
		std::cout << "getSelectedCharacter: " << game().getSelectedCharacter() << std::endl;
#endif
		game().setState(Game::RUNNING);
		}, Vector2D(startX, btnY), Vector2D(btnWidth, btnHeight), &sdlutils().images().at("ROGUE"));

	knightButton = new Button([this]() {
		selectedCharacter = "KNIGHT";
		game().setSelectedCharacter(selectedCharacter);
		game().setState(Game::RUNNING);
		}, Vector2D(startX + (btnWidth + spacing), btnY), Vector2D(btnWidth, btnHeight), &sdlutils().images().at("KNIGHT"));

	alchemistButton = new Button([this]() {
		selectedCharacter = "ALCHEMIST";
		game().setSelectedCharacter(selectedCharacter);
		game().setState(Game::RUNNING);
		}, Vector2D(startX + 2 * (btnWidth + spacing), btnY), Vector2D(btnWidth, btnHeight), &sdlutils().images().at("ALCHEMIST"));

	hunterButton = new Button([this]() {
		selectedCharacter = "HUNTER";
		game().setSelectedCharacter(selectedCharacter);
		game().setState(Game::RUNNING);
		}, Vector2D(startX + 3 * (btnWidth + spacing), btnY), Vector2D(btnWidth, btnHeight), &sdlutils().images().at("HUNTER"));
}

void NewRoundState::leave()
{
	if (selectedCharacter.empty()) {
		game().setSelectedCharacter(selectedCharacter);
	}
#ifdef _DEBUG
	std::cout << "Personaje: " << selectedCharacter << std::endl;
#endif
}

/*NewRoundState::NewRoundState() {

	// Cargar el fondo
	background = new Texture(sdlutils().renderer(), "resources/images/selectmenu-provisional.png");

	// Cargar la textura del bot�n
	buttonTexture = new Texture(sdlutils().renderer(), "resources/images/play-button.png");

	/*pressEnter = new Texture(sdlutils().renderer(),
		"Press ENTER to start the round",
		sdlutils().fonts().at("ARIAL24"), 
		build_sdlcolor(0x112233ff),
		build_sdlcolor(0xffffffff));
	
	x0 = (sdlutils().width() - pressEnter->width()) / 2;
	y0 = (sdlutils().height() - pressEnter->height()) / 2;

	// Posicionar el bot�n en el centro
	float btnWidth = buttonTexture->width();
	float btnHeight = buttonTexture->height();
	float btnX = (sdlutils().width() - btnWidth) / 2;
	float btnY = (sdlutils().height() - btnHeight) / 2;

	// Crear el bot�n con su callback
	selectButton = new Button([this]() {
		game().setState(Game::RUNNING);
		}, Vector2D(btnX, btnY), Vector2D(btnWidth, btnHeight), buttonTexture);
}*/

/*void NewRoundState::update() {
	
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
		/*if (ih().keyDownEvent()) {
			game().setState(Game::RUNNING);
			exit = true;
		}

		// Actualizar bot�n (manejo de clic)
		selectButton->update();

		// clear screen
		sdlutils().clearRenderer();

		// Dibujar el fondo
		SDL_Rect destRect = { 0, 0, sdlutils().width(), sdlutils().height() };
		background->render(destRect);

		// Dibujar el bot�n
		selectButton->render();

		// render Press Any Key
		//pressEnter->render(x0, y0);

		// present new frame
		sdlutils().presentRenderer();

		if (selectButton->isPressed()) {
			exit = true;
		}

		Uint32 frameTime = sdlutils().currRealTime() - startTime;

		if (frameTime < 20)
			SDL_Delay(20 - frameTime);
	}
}*/
