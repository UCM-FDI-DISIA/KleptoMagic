#include "EnemyDrop.h"

EnemyDrop::EnemyDrop(string name) : _upgradeName(name)
{
	
}

EnemyDrop::EnemyDrop() 
{
	_upgradeName = "rnd";
}

void EnemyDrop::initComponent() 
{
	_entTr = _ent->getMngr()->getComponent<Transform>(_ent);
	srand(time(NULL));
}

void EnemyDrop::update() 
{
	if(!_ent->getMngr()->isAlive(_ent) 
		&& (rand() % 100) <= DEFAULT_DROP_RATE)
	{
		spawnUpgrade();
	}
}

void EnemyDrop::spawnUpgrade() 
{
	ObjectUtils::Instance()->spawnRandomItem(_entTr->getPos());
}