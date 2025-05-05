#include "BulletStats.h"
#include "Game.h"
#include "../ecs/Manager.h"
BulletStats::BulletStats()
{
	_tim = new VirtualTimer();
#ifdef _DEBUG
	std::cout << _tim->currRealTime();
#endif
}

BulletStats::~BulletStats()
{
}

void BulletStats::initComponent() { //falta get entity stats speed
	auto* _mngr = _ent->getMngr();
	auto estat = _mngr->getComponent<EntityStat>(_ent);
	if (estat != nullptr) {
		_eStat = estat;
	}
	
}

float BulletStats::Created(std::string s)
{
	
	if(s=="ROGUE")
	{
		speed = 10;
		damage = 15;
		distance = 20000;
		size = 10;
		piercing = false;
		duration = distance / speed;
		bullets = 5;
		return 250;
	}
	else if(s=="KNIGHT"){
		speed = 5;
		damage = 15;
		distance = 50;
		size = 150;
		piercing = true;
		duration = distance / speed;
		return 300;
	}
	else if(s=="ALCHEMIST"){
		speed = 2.5;
		damage = 15;
		distance = 20000;
		size = 40;
		piercing = false;
		duration = distance / speed;
		return 500;
	}
	else if(s=="HUNTER"){
		speed = 5;
		damage = 15;
		distance = 999999;
		size = 50;
		piercing = true;
		duration = distance / speed;
		return 700;
	}
	
}
void BulletStats::enemyStats(int i)
{
	if(i==0)
	{
		speed = 5;
		damage = 15;
		distance = 50;
		size = 150;
		piercing = true;
		duration = distance / speed;
		
	}
	else if(i==1)
	{
		speed = 10;
		damage = 15;
		distance = 20000;
		size = 100;
		piercing = false;
		duration = distance / speed;
	}else if(i==2)
	{
		speed = 10;
		damage = 15;
		distance = 20000;
		size = 100;
		bullets = 7;
		piercing = false;
		duration = distance / speed;
	}
	else if (i == 3) {
		speed = 3;
		damage = 15;
		distance = 9000;
		size = 100;
		piercing = false;
		duration = distance / speed;
	}
	else if (i == 4) {
		speed = 2;
		damage = 15;
		distance = 6000;
		size = 100;
		piercing = false;
		duration = distance / speed;
	}
	
	//continuar cuando haya mas enemigos
}

void BulletStats::refreshStats(float spe, float dmg, float dist, float siz, bool pierc, int bull)
{
#ifdef _DEBUG
	std::cout << _tim->currRealTime();
#endif
	speed = spe;
	damage = dmg;
	distance = dist;
	size = siz;
	piercing = pierc;
	bullets = bull;
	refreshDuration();
}
void BulletStats::update()
{
	
	if (_tim->currRealTime() > duration) 
	{
		game().getMngr()->setAlive(this->_ent, false);
	}
}
