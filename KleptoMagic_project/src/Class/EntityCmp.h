#pragma once
#include "EntityStat.h"
#include "../utils/Vector2D.h"
#include "StatusEffect.h"
#include <vector>
#include "SceneObject.h"
#include <chrono>
#include <thread>
#include "../ecs/Component.h"

	class EntityCmp : public ecs::Component //lo cambiare para que solo cambie el estatus
	{
	public:
		__CMPID_DECL__(ecs::cmp::ENTITYCMP);
		Vector2D speed;
	private:


		//EntityStat* health;
		//EntityStat* movementSpeed;
		//EntityStat* damage;
		//EntityStat* attackSpeed;
		//EntityStat* shield;

		Texture* texture;
        
		std::vector<StatusEffect> statusEffects;

	public:
		EntityCmp(/*Vector2D velocity, float life, float moveSpeed, float attack, float attacksped, float armor,*/ Vector2D position, PlayState* playstate);
		void initComponent() override; 
		void addStatusEffect(StatusEffect effect);
		void updateStatusDuration();
		void calculateStatsPostStats();
		~EntityCmp();
		//virtual void move();

	};



