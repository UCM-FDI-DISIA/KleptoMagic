#include "MenuState.h"

MenuState::MenuState(Game* game)  : GameState(game) {
    screenCenter = game->getWindowWidth() / 2;
}

Button* MenuState::createButton(float x, float y, int w, int h, Game::TextureName textureName) {
    Vector2D buttonPosition = {x, y};
    Texture* buttonTexture = game->getTexture(textureName);
    if (buttonTexture == nullptr) {
#ifdef _DEBUG
        std::cerr << "Error: No se pudo cargar la textura del bot�n " << textureName << std::endl;
#endif 
    }
    Button* newButton = new Button(this, buttonTexture, buttonPosition, w, h);
    addObject(newButton);
    addEventListener(newButton);
    return newButton;
}

MenuState::~MenuState() {
}