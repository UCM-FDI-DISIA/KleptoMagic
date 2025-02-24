#pragma once
#include "../Class/GameState.h"
#include "../sdlutils/Texture.h"
class GameObject
{
private:
	GameState* gameState;
	Texture* texture;
public:
	GameObject();
	~GameObject();
	virtual void render() const;
	virtual void update();
};

