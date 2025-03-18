#pragma once
#include <SDL.h>
#include "../utils/Vector2D.h"
#include "../ecs/Component.h"
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"

const float DEFAULT_LIFE = 100;

class LifeComponent : public ecs::Component {
public:
	__CMPID_DECL__(ecs::cmp::LIFECMP);

	void init(float life);
	void setLife(float life);
	float getLife();
	float getMaxHeal();
	bool isAlive();
	void damageEntity(float damage); // se usa para recibir daño, se puede ponerse negativo para curar
	void ampliateMaxLife(float ampliation);

private:
	float _life;	// vida del personaje
	float _maxLife;	// vida máxima del personaje (se puede cambiar)
};