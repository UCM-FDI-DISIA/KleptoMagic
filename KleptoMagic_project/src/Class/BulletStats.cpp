#include "BulletStats.h"
#include "Game.h"
#include "../ecs/Manager.h"
BulletStats::BulletStats()
{
	_tim = new VirtualTimer();
}

BulletStats::~BulletStats()
{
}

void BulletStats::Created(int i)
{
	if (i == 0)
	{
		speed = 5;
		damage = 15;
		distance = 20000;
		size = 25;
		piercing = false;
		
	}
	else
	if (i == 1)
	{
		speed = 5;
		damage = 15;
		distance = 2000;
		size = 100;
		piercing = true;
	}
	else
	if (i == 2)
	{
		speed = 5;
		damage = 15;
		distance = 20000;
		size = 40;
		piercing = false;
	}
	else
	if (i == 3)
	{
		speed = 5;
		damage = 15;
		distance = 20000;
		size = 50;
		piercing = false;
	}
	duration = distance / speed;
}
void BulletStats::update()
{
	if (_tim->currRealTime() > duration) 
	{
		game().getMngr()->setAlive(this->_ent, false);
	}
}
