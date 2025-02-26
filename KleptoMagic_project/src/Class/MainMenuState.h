#pragma once
#ifndef MAIN_MENU_STATE_H
#define MAIN_MENU_STATE_H

#include "MenuState.h"
#include "../sdlutils/Texture.h"
#include "Game.h"

class MainMenuState : public MenuState {
private:
    Texture* backgroundTexture;

public:
    MainMenuState(Game* game, Texture* backgroundTexture);
    void render() const override;
};

#endif //MAIN_MENU_STATE_H