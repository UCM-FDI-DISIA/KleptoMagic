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
}

void NewRoundState::update() {
	bool exit = false;
	//auto& ihdlr = ih();
	float startX = (sdlutils().width() - (btnWidth * 4 + spacing * 3)) / 2 + 12;
	float imgY = (sdlutils().height() / 2) - btnHeight - 20 + 30; 
	int imgOffsetX = 25;

	SDL_Rect rogueRect = { static_cast<int>(startX + imgOffsetX), static_cast<int>(imgY), static_cast<int>(imgSize), static_cast<int>(imgSize) };
	SDL_Rect knightRect = { static_cast<int>(startX + (btnWidth + spacing) + imgOffsetX), static_cast<int>(imgY), static_cast<int>(imgSize), static_cast<int>(imgSize) };
	SDL_Rect alchemistRect = { static_cast<int>(startX + 2 * (btnWidth + spacing) + imgOffsetX), static_cast<int>(imgY), static_cast<int>(imgSize), static_cast<int>(imgSize) };
	SDL_Rect hunterRect = { static_cast<int>(startX + 3 * (btnWidth + spacing) + imgOffsetX), static_cast<int>(imgY), static_cast<int>(imgSize), static_cast<int>(imgSize) };

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
		rogue->render(rogueRect);
		knight->render(knightRect);
		alchemist->render(alchemistRect);
		hunter->render(hunterRect);

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

	background = new Texture(sdlutils().renderer(), "resources/images/selectMenu.png");
	rogue = new Texture(sdlutils().renderer(), "resources/images/rogue.png");
	knight = new Texture(sdlutils().renderer(), "resources/images/knight.png");
	alchemist = new Texture(sdlutils().renderer(), "resources/images/alchemist.png");
	hunter = new Texture(sdlutils().renderer(), "resources/images/hunter.png");

	float startX = (sdlutils().width() - (btnWidth * 4 + spacing * 3)) / 2 + 18;
	float btnY = sdlutils().height() / 2 - btnHeight / 2 + 30;

	rogueButton = new Button([this]() {
#ifdef _DEBUG
		std::cout << "Seleccionado: ROGUE" << std::endl; 
#endif
		selectedCharacter = "ROGUE";
		game().setSelectedCharacter(selectedCharacter);
#ifdef _DEBUG
		std::cout << "getSelectedCharacter: " << game().getSelectedCharacter() << std::endl;
#endif
		game().setState(Game::RUNNING);
		}, Vector2D(startX, btnY), Vector2D(btnWidth, btnHeight), new Texture(sdlutils().renderer(), "resources/images/select.png"), "rogueButton");

	knightButton = new Button([this]() {
		selectedCharacter = "KNIGHT";
		game().setSelectedCharacter(selectedCharacter);
		game().setState(Game::RUNNING);
		}, Vector2D(startX + (btnWidth + spacing), btnY), Vector2D(btnWidth, btnHeight), new Texture(sdlutils().renderer(), "resources/images/select.png"), "knightButton");

	alchemistButton = new Button([this]() {
		selectedCharacter = "ALCHEMIST";
		game().setSelectedCharacter(selectedCharacter);
		game().setState(Game::RUNNING);
		}, Vector2D(startX + 2 * (btnWidth + spacing), btnY), Vector2D(btnWidth, btnHeight), new Texture(sdlutils().renderer(), "resources/images/select.png"), "alchemistButton");

	hunterButton = new Button([this]() {
		selectedCharacter = "HUNTER";
		game().setSelectedCharacter(selectedCharacter);
		game().setState(Game::RUNNING);
		}, Vector2D(startX + 3 * (btnWidth + spacing), btnY), Vector2D(btnWidth, btnHeight), new Texture(sdlutils().renderer(), "resources/images/select.png"), "hunterButton");
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