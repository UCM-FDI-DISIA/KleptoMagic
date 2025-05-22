#include "BulletStats.h"
#include "../ecs/Game.h"
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
	auto* _mngr = Game::Instance()->getMngr();
	auto* player = _mngr->getHandler(ecs::hdlr::PLAYER);
	if (player != nullptr) {
		auto* entityStat = _mngr->getComponent<EntityStat>(player);
		damage = entityStat->getStat(EntityStat::Stat::Damage);
	}
	else { damage = 15; }
	if(s=="ROGUE")
	{
		speed = 10;
		
		distance = 7500;
		size = 10;
		piercing = false;
		duration = distance / speed;
		bullets = 5;
		explode = true;
		return 250;
	}
	else if(s=="KNIGHT"){
		speed = 5;
		
		distance = 50;
		size = 150;
		piercing = true;
		duration = distance / speed;
		explode = false;
		return 300;
	}
	else if(s=="ALCHEMIST"){
		speed = 2.5;
		explode = true;
		distance = 10000;
		size = 40;
		piercing = false;
		duration = distance / speed;
		return 500;
	}
	else if(s=="HUNTER"){
		speed = 20;
		explode = false;
		distance = 90000;
		size = 15;
		piercing = true;
		duration = distance / speed;
		return 700;
	}
	
}
void BulletStats::explosionStats(int dam)
{
	damage = dam;
	damageMul = 1;
	size = 120;
	distance = 75;
	speed = 1;
	duration = distance / speed;
	piercing = true;
	explode = false;
}
void BulletStats::enemyStats(int i)
{
	auto* entityStat = Game::Instance()->getMngr()->getComponent<EntityStat>(_ent);
	damage = entityStat->getStat(EntityStat::Stat::Damage);
	if(i==0)
	{
		speed = 5;
		
		distance = 50;
		size = 150;
		piercing = true;
		duration = distance / speed;
		
	}
	else if(i==1)
	{
		speed = 10;
		
		distance = 20000;
		size = 100;
		piercing = false;
		duration = distance / speed;
	}else if(i==2)
	{
		speed = 10;
		
		distance = 20000;
		size = 100;
		bullets = 7;
		piercing = false;
		duration = distance / speed;
	}
	else if (i == 3) {
		speed = 3;
		
		distance = 9000;
		size = 100;
		piercing = false;
		duration = distance / speed;
	}
	else if (i == 4) {
		speed = 2;
		
		distance = 6000;
		size = 100;
		piercing = false;
		duration = distance / speed;
	}
	
	//continuar cuando haya mas enemigos
}

void BulletStats::refreshStats(float spe, float dmg, float dist, float siz, bool pierc,bool explo, int bull, float slowt, float slows, float stunt, float dott, float dots)
{
	auto* _mngr = Game::Instance()->getMngr();
	auto* player = _mngr->getHandler(ecs::hdlr::PLAYER);
#ifdef _DEBUG
	std::cout << _tim->currRealTime();
#endif
	speed = spe;
	if (player != nullptr) {
		auto* entityStat = _mngr->getComponent<EntityStat>(player);
		damage = entityStat->getStat(EntityStat::Stat::Damage);
	}
	else { damage = dmg; }
	distance = dist;
	size = siz;
	piercing = pierc;
	explode = explo;
	bullets = bull;
	slowT = slowt;
	slowS = slows;
	stunT = stunt;
	DotT = dott;
	DotS = dots;
	refreshDuration();
}

void BulletStats::update()
{
	
	if (_tim->currRealTime() > duration) 
	{
		game().getMngr()->setAlive(this->_ent, false);
	}
}
