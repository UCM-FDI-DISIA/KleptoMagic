#include "GameOverState.h"
#include "../state/NewGameState.h"
#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/NewInputHandler.h"

GameOverState::GameOverState() {

}
GameOverState::~GameOverState() {

}

void GameOverState::enter() {

	// Cargar el fondo
	background = new Texture(sdlutils().renderer(), "resources/images/endMenu.png");

	homeTexture = new Texture(sdlutils().renderer(), "resources/images/home.png");

	winText = new Texture(sdlutils().renderer(), "resources/images/winText.png");
	loseText = new Texture(sdlutils().renderer(), "resources/images/loseText.png");

	float btnWidth = homeTexture->width() / 4;
	float btnHeight = homeTexture->height() / 4;

	float playBtnX = (sdlutils().width() - btnWidth) / 2;
	float playBtnY = (sdlutils().height() - btnHeight) / 2 + 50;

	homeButton = new Button([this]() {
		auto player = game().getMngr()->getHandler(ecs::hdlr::PLAYER);
		if (game().getMngr()->isAlive(player)) {
			game().getMngr()->setAlive(player, false); 
		}
		game().pushState(new NewGameState());
		}, Vector2D(playBtnX, playBtnY + 100), Vector2D(btnWidth, btnHeight), homeTexture, "button");
}

void GameOverState::update() {
	
	bool exit = false;

	// reset the time before starting - so we calculate correct
	// delta-time in the first iteration
	//
	sdlutils().resetTime();

	while (!exit) {
		Uint32 startTime = sdlutils().currRealTime();

		// update the event handler
		NewInputHandler::Instance()->update();

		homeButton->update();

		if (homeButton->isPressed()) {
			exit = true; 
		}

		// clear screen
		sdlutils().clearRenderer();

		// Render background picture
		background->render({ 0, 0, sdlutils().width(), sdlutils().height() });

		// Render win or lose text based on game result
		float textWidth = winText->width() / 4;
		float textHeight = winText->height() / 4;
		float textX = (sdlutils().width() - textWidth) / 2;
		float textY = (sdlutils().height() - textHeight) / 2 - 50;

		if (game().getResult()) {
			winText->render({ (int)textX + 20, (int)textY, (int)textWidth, (int)textHeight });
		}
		else {
			loseText->render({ (int)textX - 25, (int)textY, (int)textWidth, (int)textHeight });
		}

		homeButton->render();

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
