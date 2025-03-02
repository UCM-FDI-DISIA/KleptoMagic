#pragma once
#include "EntityStat.h"
#include "../utils/Vector2D.h"
#include "StatusEffect.h"
#include <vector>
#include "SceneObject.h"
#include <chrono>
#include <thread>

	class Entity : public SceneObject
	{
		Vector2D speed;


		EntityStat* health;
		EntityStat* movementSpeed;
		EntityStat* damage;
		EntityStat* attackSpeed;
		EntityStat* shield;

		Texture* texture;
        
		std::vector<StatusEffect> statusEffects;

	public:
		Entity(Vector2D velocity,float life, float moveSpeed, float attack, float attacksped, float armor,Vector2D position,PlayState* playstate);
		void addStatusEffect(StatusEffect effect);
		void updateStatusDuration();
		void calculateStatsPostStats();
		~Entity();
		//virtual void move();

	};



