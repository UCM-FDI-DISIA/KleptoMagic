#include "Bullet.h"
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"


Bullet::~Bullet()
{
	
}

void Bullet::initComponent()
{
	//_tr = Game::Instance()->getMngr()->getComponent<Transform>(this->_ent);
	auto* _mngr = _ent->getMngr();
	_tr = _mngr->getComponent<Transform>(_ent);
	assert(_tr != nullptr);
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

void Bullet::shoot(int index)
{
	int x;
	int y;
	SDL_GetMouseState(&x, &y);
	float xf = static_cast<float>(x);
	float yf = static_cast<float>(y);
	Vector2D PosRat = { xf,yf};
	_bullets[index].used = true;
	_bullets[index].pos.set({ _tr->getPos().getX() + _tr->getWidth() / 2,_tr->getPos().getY() + _tr->getHeight() / 2 });
	_bullets[index].rot = PosRat.angle(_bullets[index].pos);
	_bullets[index].vel = Vector2D(0, -1).rotate(_bullets[index].rot) * 10.0f;
	// falta rotacion Idea: pos del raton - posición del personaje normalizado
	std::cout << "pew";

}
