#include "Bullet.h"



Bullet::Bullet()
{
	_tim = new VirtualTimer();
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
	
	if (_tim->currRealTime() > 250) {
		shoot();
	}
}

void Bullet::generateBullets()
{
}

void Bullet::shoot()
{
	auto* _mngr = game().getMngr();

	Transform* _tr = _mngr->getComponent<Transform>(_mngr->getHandler(ecs::hdlr::PLAYER));
	int x, y;
	SDL_GetMouseState(&x, &y);
	float xf = static_cast<float>(x);
	float yf = static_cast<float>(y);
	Vector2D PosRat = { xf,yf };
	
	auto _bullets = _mngr->addEntity();
	_mngr->setHandler(ecs::grp::BULLET, _bullets);
	auto* stats = _mngr->addComponent<BulletStats>(_bullets);
	stats->Created(0);
	Vector2D vel = (PosRat - _tr->getPos()).normalize() * stats->getSpeed();
	auto _bulletsTR = _mngr->addComponent<Transform>(_bullets);
	_bulletsTR->init(_tr->getPos(), vel, stats->getSize(), stats->getSize(), 0);
	_mngr->addComponent<ImageWithFrames>(_bullets, tex, 1, 1);
	_tim->resetTime();

}
