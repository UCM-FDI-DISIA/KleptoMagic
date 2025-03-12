#include "PlayState.h"

PlayState::PlayState(Game*  game) : GameState(game)
{
	currentRoom = 0;
	coins = 0;
	timer = 500;
	player = new Player(game,1, 1, 1, Vector2D(0,0), 100, 1, 1, 1, 1, Vector2D(100,100), this);
	gameObj.push_back(player);
	addEventListener(player);
}

void PlayState::addObject(SceneObject* object)
{
	sceneObjects.push_back(object);
}

Player* PlayState::getPlayer()
{
	return player;
}

//usa el Collision.h para comprobar si hay colision entre dos objetos
bool PlayState::CheckCollision()
{
	std::list<SceneObject*>::iterator it1 = sceneObjects.begin();
	std::list<SceneObject*>::iterator it2 = sceneObjects.begin();
	it2++;
	while (it1 != sceneObjects.end())
	{
		while (it2 != sceneObjects.end())
		{
			if (Collisions::collidesWithRotation((*it1)->GetPos(), (*it1)->GetWidth(), (*it1)->GetHeight(),0, (*it2)->GetPos(), (*it2)->GetWidth(), (*it2)->GetHeight(), 0))
			{
				return true;
			}
			it2++;
		}
		it1++;
		it2 = it1;
		it2++;
	}
	return false;
}

