#include "EndMenuState.h"

EndMenuState::EndMenuState(Game* game, Texture* background) : MenuState(game) 
{
	backgroundTexture = game->getTexture(Game::ENDMENUBACKGROUND);

	Button* returnButton = createButton(game->getWindowWidth() / 2 - 50, game->getWindowHeight() / 2 - 25, 100, 50, Game::ENDMENUBUTTON);

}

EndMenuState::~EndMenuState()
{
	// No need to delete backgroundTexture, it is managed by the Game class
   // Delete buttons and other game objects if necessary
	for (GameObject* gameObject : gameObj)
	{
		delete gameObject;
	}
	gameObj.clear();
}
void EndMenuState::render() const 
{
	SDL_Renderer* renderer = game->getRenderer();
	SDL_Rect rect = { 0, 0, game->getWindowWidth(), game->getWindowHeight()};
	if (backgroundTexture != nullptr) 
	{
		backgroundTexture->render(rect);
	}
	else 
	{
#ifdef _DEBUG
		std::cerr << "Error: backgroundTexture es nullptr en EndMenuState" << std::endl;
#endif
	}
	for (const GameObject* gameObject : gameObj)
	{
		gameObject->render();
	}
}