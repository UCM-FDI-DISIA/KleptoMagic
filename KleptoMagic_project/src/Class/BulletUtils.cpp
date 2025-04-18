#include "BulletUtils.h"



BulletUtils::BulletUtils()
{
	_tim = new VirtualTimer();
	for(int i=0;i<componentes.size();i++)
	{
		componentes[i] = false;
	}
	player = game().getSelectedCharacter();
	attSpeedCapFlat= bulStat->Created(player);
}

BulletUtils::~BulletUtils()
{
	
}

void BulletUtils::update()
{
	auto* mngr = game().getMngr();
	for (auto bull : mngr->getEntities(ecs::grp::BULLET))
	{
		auto bullStat = mngr->getComponent<BulletStats>(bull);
		if (!bullStat->getPiercing() && _dungeonfloor != nullptr)
		{
			auto tilecollision = mngr->getComponent<TileCollisionChecker>(bull);
			if (!tilecollision->getCanMove()) { mngr->setAlive(bull,false); }
		}
	}
}

void BulletUtils::render()
{
}

void BulletUtils::reset()
{
	auto* mngr = game().getMngr();
	for (auto bull : mngr->getEntities(ecs::grp::BULLET))
	{
		mngr->setAlive(bull, false);
	}
}

void BulletUtils::hit(int index)
{
}

void BulletUtils::pressed()
{
	
	if (_tim->currRealTime() > attSpeedCapFlat*attSpeedCapMul) {
		shoot();
	}
}

void BulletUtils::enemyShoot(Transform* _enemyTR, int i)
{
	auto* _mngr = game().getMngr();
	Transform* _tr = _mngr->getComponent<Transform>(_mngr->getHandler(ecs::hdlr::PLAYER));
	
	
	auto _bullets = _mngr->addEntity(ecs::grp::BULLET);
	auto* stats = _mngr->addComponent<BulletStats>(_bullets);
	stats->enemyStats(i);
	Vector2D vel = Vector2D(_tr->getPos() - _enemyTR->getPos()).normalize()*stats->getSpeed();
	float rot = -vel.normalize().angle(Vector2D(0, -1));
	auto* _bulletsTR = _mngr->addComponent<Transform>(_bullets);
	_bulletsTR->init(Vector2D(_enemyTR->getPos().getX() + _enemyTR->getWidth() / 2, _enemyTR->getPos().getY() + _enemyTR->getHeight() / 2) - Vector2D(stats->getSize() / 2, stats->getSize() / 2), vel, stats->getSize(), stats->getSize(), rot);
	//la imagen debe de ser distinta para cada enemigo
	_mngr->addComponent<ImageWithFrames>(_bullets, tex, 1, 1);
	_mngr->addComponent<DestroyOnBorder>(_bullets);
	if (!stats->getPiercing())
	{
		auto tilechecker = _mngr->addComponent<TileCollisionChecker>(_bullets);
		tilechecker->init(true, _bulletsTR, _dungeonfloor);
		_bulletsTR->initTileChecker(tilechecker);
	}

}

void BulletUtils::collided(ecs::entity_t e)
{
	auto* mngr = game().getMngr();
	auto bullStat = mngr->getComponent<BulletStats>(e);
	if(!bullStat->getPiercing())
	{
		mngr->setAlive(e, false);
	}
}

void BulletUtils::addComponent(int i)
{
	componentes[i] = true;
}

void BulletUtils::checkComponent(int i, ecs::entity_t bullet)
{
	auto* _mngr = game().getMngr();
	if(i==0)
	{
		_mngr->addComponent<HomingComponent>(bullet);
	}
}



void BulletUtils::shoot()
{
	auto* _mngr = game().getMngr();

	Transform* _tr = _mngr->getComponent<Transform>(_mngr->getHandler(ecs::hdlr::PLAYER));
	int x, y;
	SDL_GetMouseState(&x, &y);
	float xf = static_cast<float>(x);
	float yf = static_cast<float>(y);
	Vector2D PosRat = { xf,yf };
	
	auto _bullets = _mngr->addEntity(ecs::grp::BULLET);
	auto* stats = _mngr->addComponent<BulletStats>(_bullets);
	stats->refreshStats(bulStat->getSpeed(),bulStat->getDamage(),bulStat->getDistance(),bulStat->getSize(),bulStat->getPiercing());
	Vector2D vel = (PosRat - Vector2D(_tr->getPos().getX()+(_tr->getWidth()/2), _tr->getPos().getY()+(_tr->getHeight()/2))).normalize() * stats->getSpeed();
	float rot = -vel.normalize().angle(Vector2D(0, -1));
	std::cout << rot<<'\n';
	auto _bulletsTR = _mngr->addComponent<Transform>(_bullets);
	_bulletsTR->init(Vector2D(_tr->getPos().getX() + _tr->getWidth() / 2, _tr->getPos().getY() + _tr->getHeight() / 2)-Vector2D(stats->getSize()/2,stats->getSize()/2), vel, stats->getSize(), stats->getSize(), rot);
	_mngr->addComponent<ImageWithFrames>(_bullets, tex, 1, 1);
	_mngr->addComponent<DestroyOnBorder>(_bullets);
	for(int i=0;i<componentes.size();i++)
	{
		if (componentes[i]) { checkComponent(i,_bullets); }
	}
	if(!bulStat->getPiercing())
	{
		auto tilechecker = _mngr->addComponent<TileCollisionChecker>(_bullets);
		tilechecker->init(true, _bulletsTR, _dungeonfloor);
		_bulletsTR->initTileChecker(tilechecker);
	}
	_tim->resetTime();

}
