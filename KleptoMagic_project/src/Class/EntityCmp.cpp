#include "EntityCmp.h"

EntityCmp:: EntityCmp(Vector2D velocity, float life, float moveSpeed, float attack, float attacksped, float armor,Vector2D position,PlayState* playstate) : SceneObject (position,playstate, texture, 100, 100) //cambiar 0 0 por variables
{
	health = new EntityStat(life);
	movementSpeed = new EntityStat(moveSpeed);
	damage = new EntityStat(attack);
	attackSpeed = new EntityStat(attacksped);
	shield = new EntityStat(armor);
	
}
EntityCmp::~EntityCmp() {}
void EntityCmp:: addStatusEffect(StatusEffect effect) 
{
	statusEffects.push_back(effect);
}
void EntityCmp::calculateStatsPostStats()
{

}
void EntityCmp::updateStatusDuration()
{
	for (int i = 0; i < statusEffects.size(); i++)
	{
        int lefttime = statusEffects[i].DecreaseTime();
		if (lefttime == 0)
		{
			statusEffects.erase(statusEffects.begin() + i);
		}
	}
	std::this_thread::sleep_for(std::chrono::seconds(1));
}
