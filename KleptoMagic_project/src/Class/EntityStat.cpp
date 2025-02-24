#include "EntityStat.h"
EntityStat::EntityStat(float initial)
{
	
	iniStat = initial;
	endStat = initial;
	//se setean a su minimo
	multi = 1;
	addi = 0;
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
	//se suma al addi el dd y se resetea
	addi += add;
	Reset();
}

void EntityStat::ChangeMulti(float mult)
{
	//se suma el nuevo multiplicador y luego se resetea
	multi += mult;
	Reset();
}

