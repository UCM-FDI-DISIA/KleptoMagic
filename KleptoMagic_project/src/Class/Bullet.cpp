#include "Bullet.h"
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "../Class/ImageWithFrames.h"


Bullet::Bullet()
{
}

Bullet::~Bullet()
{
	
}

void Bullet::update()
{
}

void Bullet::render()
{
}

void Bullet::reset()
{
}

void Bullet::hit(int index)
{
}

void Bullet::pressed()
{
	bool disparado = false;
	int i = 0;
	while (i < _max_bullets && !disparado)
	{
		if (!_bullets[i].used) 
		{
			shoot(i)
			; disparado = true; 
		}
		i++;
	}
}

void Bullet::generateBullets()
{
	auto tex = &sdlutils().images().at("star");
	auto* _mngr = _ent->getMngr();
	for(int i =0; i<_bullets.size();i++)
	{
		
		_bullets[i] = _mngr->addEntity();
		_mngr->setHandler(ecs::grp::BULLET,_bullets[i]);
		_mngr->addComponent<ImageWithFrames>(_bullets[i],tex,1,1);
		_bulletsTR[i] = _mngr->addComponent<Transform>(_bullets[i])
		_bulletsTR[i]->init({-1.0f,-20.0f},{12,0}, 25, 25, 0.0f);
		//auto bullet=_mngr->
	}
}

void Bullet::shoot(int index)
{
	

}
