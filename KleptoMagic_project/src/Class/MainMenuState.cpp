#include "MainMenuState.h"
#include "Button.h"
#include "MenuState.h"

MainMenuState::MainMenuState(Game* game, Texture* background) : MenuState(game){
    backgroundTexture = game->getTexture(Game::BACKGROUND);
    if (backgroundTexture == nullptr) {
        std::cerr << "Error: No se pudo cargar la textura de fondo." << std::endl;
    }
    Button* playButton = createButton(game->getWindowWidth() / 2 - 50,
        game->getWindowHeight() / 2 - 25,
        100, 50,
        Game::BUTTON);
}


void MainMenuState::render() const {
    SDL_Renderer* renderer = game->getRenderer();
    SDL_Rect rect = { 0, 0, game->getWindowWidth(), game->getWindowHeight() };
    if (backgroundTexture != nullptr) {
        backgroundTexture->render(rect);
    }
    else {
        std::cerr << "Error: backgroundTexture es nullptr en MainMenuState::render()" << std::endl;
    }
    for (const GameObject* gameObject : gameObj) {
        gameObject->render();
    }
}