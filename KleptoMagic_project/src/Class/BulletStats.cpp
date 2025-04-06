#include "BulletStats.h"

BulletStats::BulletStats()
{
	
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
		distance = 20;
		size = 25;
		piercing = false;
	}
	else
	if (i == 1)
	{
		speed = 5;
		damage = 15;
		distance = 2;
		size = 100;
		piercing = true;
	}
	else
	if (i == 2)
	{
		speed = 5;
		damage = 15;
		distance = 20;
		size = 40;
		piercing = false;
	}
	else
	if (i == 3)
	{
		speed = 5;
		damage = 15;
		distance = 20;
		size = 50;
		piercing = false;
	}
}
