#include "../src/game/LifeComponent.h"

void LifeComponent::init(float life) {
	_life = life;
	_maxLife = life;
}

void LifeComponent::setLife(float life) {
	_life = life;
}

float LifeComponent::getLife() {
	return _life;
}

bool LifeComponent::isAlive() {
	return _life <= 0;
}

void LifeComponent::damageEntity(float damage) {
	_life = _life - damage;

	if (_life > _maxLife) _life = _maxLife;
}

void LifeComponent::ampliateMaxLife(float ampliation) {
	_maxLife = _maxLife + ampliation;
}