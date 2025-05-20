#include "../ecs/Component.h"
#include "../ecs/Manager.h"
#pragma once

class EntityStat : public ecs::Component//hay que meter getters y setters
{
public:
	__CMPID_DECL__(ecs::cmp::ENTITYSTAT);

	//enum class for every stat
	enum class Stat {
		HealthTotal,
		HealthCurrent,
		MovementSpeed,
		Damage,
		AttackSpeed,
		Shield
	};

	EntityStat(float health, float shld, float speed, float dmg, float aspd);
	~EntityStat();
	void Reset();
	void ChangeStat(float add, Stat stat);
	void ChangeMulti(float mult , Stat stat);

	//void initComponent() override;
	void update() override;

	bool isDead() { return healthCurrent <= 0; };

	float getStat(Stat stat) const {
		switch (stat) {
		case Stat::HealthTotal: return healthTotal;
		case Stat::HealthCurrent: return healthCurrent;
		case Stat::MovementSpeed: return movementSpeed * movementSpeedMult;
		case Stat::Damage: return damage * damageMult;
		case Stat::AttackSpeed: return attackSpeed * attackSpeedMult;
		case Stat::Shield: return shield;
		default: return 0.0f;
		}
	}

private:
	void createStart();
	//stats iniciales por si hace flata recalcular en caso de valor imposible o excepcion
	float healthTotalIni;
	float movementSpeedIni;
	float damageIni;
	float attackSpeedIni;

	//Vida
	float healthTotal;
	float healthCurrent;
	float shield;
	float healthMax; //para  no pasarnos de X vida maxima

	//Velocidad de movimiento (podria ser negativa? XD)
	float movementSpeed;
	float movementSpeedMult;

	//Daño
	float damage;
	float damageMult;

	//Velocidad de ataque
	float attackSpeed;
	float attackSpeedMult;
};

