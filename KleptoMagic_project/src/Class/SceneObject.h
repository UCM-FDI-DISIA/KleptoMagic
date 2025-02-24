#pragma once
//#include "Playstate"
#include "../utils/Vector2D.h"
#include "GameObject.h"
//#include "collider"
class SceneObject:public GameObject
{
private:
	//Playstate* playstate;
	Vector2D position;
	//Collider collider;
public:
	SceneObject(Vector2D ini);
	~SceneObject();
};

