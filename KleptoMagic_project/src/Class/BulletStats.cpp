#include "BulletStats.h"
#include "Game.h"
#include "../ecs/Manager.h"
BulletStats::BulletStats()
{
	_tim = new VirtualTimer();
	std::cout << _tim->currRealTime();
}

BulletStats::~BulletStats()
{
}

float BulletStats::Created(std::string s)
{
	
	if(s=="ROGUE")
	{
		speed = 10;
		damage = 15;
		distance = 20000;
		size = 100;
		piercing = false;
		duration = distance / speed;
		return 250;
	}
	else if(s=="KNIGHT"){
		speed = 5;
		damage = 15;
		distance = 200;
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
void BulletStats::refreshStats(float spe, float dmg, float dist, float siz, bool pierc)
{
	std::cout << _tim->currRealTime();
	speed = spe;
	damage = dmg;
	distance = dist;
	size = siz;
	piercing = pierc;
	refreshDuration();
}
void BulletStats::update()
{
	
	if (_tim->currTime() > duration) 
	{
		game().getMngr()->setAlive(this->_ent, false);
	}
}
