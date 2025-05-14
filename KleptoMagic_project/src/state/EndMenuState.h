#pragma once
#ifndef ENDMENUSTATE_H
#define ENDMENUSTATE_H

#include "MenuState.h"
#include "Button.h"
#include "../sdlutils/Texture.h"
#include "GameState.h"

class EndMenuState : public MenuState 
{
private:
	Texture* backgroundTexture;

public:
	EndMenuState(Game* game, Texture* backgroundTexture);
	void render() const override;
};
#endif //ENDMENUSTATE_H