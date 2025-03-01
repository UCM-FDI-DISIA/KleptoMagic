#pragma once
#include "../Class/GameState.h"
#include "../sdlutils/Texture.h"
class GameObject
{
private:
	GameState* state;
	Texture* texture;
public:
	GameObject(GameState* game, Texture* tex);
	~GameObject();
	virtual void render() const;
	virtual void update();
};

