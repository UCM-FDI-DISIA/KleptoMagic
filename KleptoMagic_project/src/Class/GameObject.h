#pragma once
#include "GameState.h"
#include "../sdlutils/Texture.h"


class GameObject
{
protected:
	GameState* state;
	Texture* texture;
public:
	GameObject(GameState* game, Texture* tex);
	~GameObject();
	virtual void render() const;
	virtual void update();
};

