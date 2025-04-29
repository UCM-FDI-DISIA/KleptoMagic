#include "EntityStat.h"

EntityStat::EntityStat(float health, float shld, float speed, float dmg, float aspd): healthTotalIni(health), shield(shld), movementSpeedIni(speed), damageIni(dmg), attackSpeedIni(aspd)
{
    healthCurrent = healthTotal = healthTotalIni;
    movementSpeed = movementSpeedIni;
    damage = damageIni;
    attackSpeed = attackSpeedIni;

    movementSpeedMult = damageMult = attackSpeedMult = 1;
	//iniStat = initial;
	//endStat = initial;
	////se setean a su minimo
	//multi = 1;
	//addi = 0;
}

EntityStat::~EntityStat()
{
}

void EntityStat::Reset()
{
	//endStat = (iniStat * multi) + addi;
}

void EntityStat::ChangeFlat(float add, Stat stat)
{
	//se suma al addi el dd y se resetea
    switch (stat) {
    case Stat::HealthTotal:
        healthCurrent += add;
        if (healthTotal < healthMax) healthCurrent = healthTotal;
        break;
    case Stat::HealthCurrent:
        healthCurrent += add;
        if (healthTotal < healthMax) healthCurrent = healthTotal;
        break;
    case Stat::MovementSpeed:
        movementSpeed += add;
        break;
    case Stat::Damage:
        damage += add;
        break;
    case Stat::AttackSpeed:
        attackSpeed += add;
        break;
    case Stat::Shield:
        shield += add;
        break;
    }
}

void EntityStat::ChangeMulti(float mult, Stat stat)
{
	//se suma el nuevo multiplicador y luego se resetea
    //se suma al addi el dd y se resetea
    switch (stat) {
    case Stat::HealthTotal:
        throw "No hay multiplicador de healthTotal";
        break;
    case Stat::HealthCurrent:
        throw "No hay multiplicador de healthCurrent";
        break;
    case Stat::MovementSpeed:
        movementSpeedMult += mult;
        break;
    case Stat::Damage:
        damageMult += mult;
        break;
    case Stat::AttackSpeed:
        attackSpeedMult += mult;
        break;
    case Stat::Shield:
        throw "No hay multiplicador de shield";
        break;
    }
}
