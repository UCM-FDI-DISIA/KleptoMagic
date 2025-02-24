#include "EntityStat.h"
EntityStat::EntityStat(float initial)
{
	iniStat = initial;
}

EntityStat::~EntityStat()
{
}

void EntityStat::Reset()
{
	endStat = (iniStat * multi) + addi;
}

void EntityStat::ChangeAddi(float add)
{
	addi += add;
	Reset();
}

void EntityStat::ChangeMulti(float mult)
{
	multi += mult;
	Reset();
}

