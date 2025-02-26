#include "PlayState.h"

PlayState::PlayState()
{
	currentRoom = 0;
	coins = 0;
	timer = 500;

}


PlayState::~PlayState()
{
}

void PlayState::addObject(SceneObject* object)
{
	sceneObjects.push_back(object);
}


//usa el Collision.h para comprobar si hay colision entre dos objetos
bool PlayState :: CheckCollision()