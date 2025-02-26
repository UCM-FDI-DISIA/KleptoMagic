#pragma once
#include "PlayState.h"
#include "../utils/Vector2D.h"
#include "GameObject.h"

class SceneObject:public GameObject
{
private:
	PlayState* playstate;
	Vector2D position;
	float width; float height;
public:
	SceneObject(Vector2D ini,PlayState* pointer,float w, float h);
	~SceneObject();
	Vector2D GetPos() { return position; }
	float GetWidth() { return width; }
	float GetHeight() { return height; }
};

