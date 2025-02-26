#include "SceneObject.h"

SceneObject::SceneObject(Vector2D ini,PlayState*pointer)
{
	position = ini;
	playstate = pointer;
}

SceneObject::~SceneObject()
{
}
