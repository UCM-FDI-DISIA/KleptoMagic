#include "SceneObject.h"

#include"PlayState.h"

SceneObject::SceneObject(Vector2D ini,PlayState*pointer,float w, float h) : GameObject(playstate)
{
	position = ini;
	playstate = pointer;
	width = w;
	height = h;
}

SceneObject::~SceneObject()
{
}
