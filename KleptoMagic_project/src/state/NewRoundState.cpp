#include "NewRoundState.h"
#include "../state/RunningState.h"
#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/NewInputHandler.h"
#include "../sdlutils/SDLUtils.h"

NewRoundState::NewRoundState() : selectedCharacter("") {
#ifdef _DEBUG
	std::cout << "Nuevo NewRoundState creado!" << std::endl;
#endif
}

void NewRoundState::update() {
	bool exit = false;
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
	delete background;
	delete rogue;
	delete knight;
	delete alchemist;
	delete hunter;

	delete rogueButtonTexture;
	delete knightButtonTexture;
	delete alchemistButtonTexture;
	delete hunterButtonTexture;

	delete rogueButton;
	delete knightButton;
	delete alchemistButton;
	delete hunterButton;
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

	rogueButtonTexture = new Texture(sdlutils().renderer(), "resources/images/select.png");
	knightButtonTexture = new Texture(sdlutils().renderer(), "resources/images/select.png");
	alchemistButtonTexture = new Texture(sdlutils().renderer(), "resources/images/select.png");
	hunterButtonTexture = new Texture(sdlutils().renderer(), "resources/images/select.png");


	rogueButton = new Button([this]() {
#ifdef _DEBUG
		std::cout << "Seleccionado: ROGUE" << std::endl;
#endif
		selectedCharacter = "ROGUE";
		game().setSelectedCharacter(selectedCharacter);
		game().pushState(new RunningState());
		}, Vector2D(startX, btnY), Vector2D(btnWidth, btnHeight), rogueButtonTexture, "rogueButton");

	knightButton = new Button([this]() {
		selectedCharacter = "KNIGHT";
		game().setSelectedCharacter(selectedCharacter);
		game().pushState(new RunningState());
		}, Vector2D(startX + (btnWidth + spacing), btnY), Vector2D(btnWidth, btnHeight), knightButtonTexture, "knightButton");

	alchemistButton = new Button([this]() {
		selectedCharacter = "ALCHEMIST";
		game().setSelectedCharacter(selectedCharacter);
		game().pushState(new RunningState());
		}, Vector2D(startX + 2 * (btnWidth + spacing), btnY), Vector2D(btnWidth, btnHeight), alchemistButtonTexture, "alchemistButton");

	hunterButton = new Button([this]() {
		selectedCharacter = "HUNTER";
		game().setSelectedCharacter(selectedCharacter);
		game().pushState(new RunningState());
		}, Vector2D(startX + 3 * (btnWidth + spacing), btnY), Vector2D(btnWidth, btnHeight), hunterButtonTexture, "hunterButton");
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