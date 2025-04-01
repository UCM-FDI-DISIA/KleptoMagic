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
	/*auto* mngr = game().getMngr();
	for(auto* _bullets : mngr->getEntities(ecs::grp::BULLET))
	{
		auto _bulletsTR = mngr->getComponent<Transform>(_bullets);
	}*/
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

void Bullet::pressed(int i)
{
	
	if (_tim->currRealTime() > 250) {
		shoot(i);
	}
}

void Bullet::collided(ecs::entity_t e)
{
	auto* mngr = game().getMngr();
	auto bullStat = mngr->getComponent<BulletStats>(e);
	if(!bullStat->getPiercing())
	{
		mngr->setAlive(e, false);
	}
}



void Bullet::shoot(int i)
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
	stats->Created(i);
	Vector2D vel = (PosRat - Vector2D(_tr->getPos().getX()+(_tr->getWidth()/2), _tr->getPos().getY()+(_tr->getHeight()/2))).normalize() * stats->getSpeed();
	auto _bulletsTR = _mngr->addComponent<Transform>(_bullets);
	_bulletsTR->init(Vector2D(_tr->getPos().getX() + (_tr->getWidth() / 2), _tr->getPos().getY() +( _tr->getHeight() / 2)), vel, stats->getSize(), stats->getSize(), 0);
	_mngr->addComponent<ImageWithFrames>(_bullets, tex, 1, 1);
	_mngr->addComponent<DestroyOnBorder>(_bullets);
	_mngr->addComponent<HomingComponent>(_bullets);
	_tim->resetTime();

}
