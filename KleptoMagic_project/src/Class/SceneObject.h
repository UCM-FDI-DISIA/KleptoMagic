#pragma once
#include "PlayState.h"
#include "../utils/Vector2D.h"
#include "GameObject.h"
//#include "collider"
class SceneObject:public GameObject
{
private:
	PlayState* playstate;
	Vector2D position;
	//Collider collider;
public:
	SceneObject(Vector2D ini,PlayState* pointer);
	~SceneObject();
};

