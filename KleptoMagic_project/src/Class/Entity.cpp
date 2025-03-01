#include "Entity.h"

Entity:: Entity(Vector2D velocity, float life, float moveSpeed, float attack, float attacksped, float armor,Vector2D position,PlayState* playstate):SceneObject(position,playstate)
{
	health = new EntityStat(life);
	movementSpeed = new EntityStat(moveSpeed);
	damage = new EntityStat(attack);
	attackSpeed = new EntityStat(attacksped);
	shield = new EntityStat(armor);
	
}
void Entity:: addStatusEffect(StatusEffect effect) 
{
	statusEffects.push_back(effect);
}
void Entity::calculateStatsPostStats()
{

}
void Entity::updateStatusDuration()
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
