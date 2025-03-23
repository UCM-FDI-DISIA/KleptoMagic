#include "../ecs/Component.h"
#include "../ecs/ecs.h"
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
#include "../ecs/ecs_defs_example.h"
#include "Transform.h"
#include <chrono>
namespace ecs 
{
	class SlimeVectorComponent : public Component
	{
	public:
		float direcionX, direcionY;
		void CreateVector(Vector2D playerPos, Vector2D enemyPos) {
			direcionX = playerPos.getX() - enemyPos.getX();
			direcionY = playerPos.getY() - enemyPos.getY();
			float length = std::sqrt(direcionX * direcionX + direcionY * direcionY);
			if (length != 0) {
				direcionX /= length;
				direcionY /= length;
			}
		}
	};
	class SlimeStatComponent : public Component 
	{
	public:
		float speed = 50;
		float damage = 10;
		float attackspeed = 10;
		void update() override {}
	};
	class SlimeMovementComponent : public Component
	{
		Entity* player = nullptr;
		
		void update() override
		{
			if (player != nullptr)
			{
				auto vector = static_cast<SlimeVectorComponent*>(_ent->getMngr()->getComponent<SlimeVectorComponent>(_ent));
				auto stat = static_cast<SlimeStatComponent*>(_ent->getMngr()->getComponent<SlimeStatComponent>(_ent));
				auto _transform = static_cast<Transform*>(_ent->getMngr()->getComponent<Transform>(_ent));
				
				if (vector && stat && _transform)
				{
					vector->CreateVector(_transform->getPos(), _player->getPos());
					Vector2D velocity(vector->direcionX * stat->speed, vector->direcionY * stat->speed);
				}
			}
		
		}
	};
	class CollideComponent : public Component
	{

	};
	class RenderComponent : public Component
	{

	};


	class SlimeAttackComponent : public Component
	{
	public:
		Entity* player = nullptr;
		float attackCooldown;
		std::chrono::steady_clock::time_point lastAttackTime = std::chrono::steady_clock::now();
		void update() override
		{


			auto stat = static_cast<SlimeStatComponent*>(_ent->getMngr()->getComponent<SlimeStatComponent>(_ent));
			auto _transform = static_cast<Transform*>(_ent->getMngr()->getComponent<Transform>(_ent));
			auto _player = static_cast<Transform*>(_ent->getMngr()->getComponent<Transform>(player));
			attackCooldown = 10 - stat->attackspeed;
			auto now = std::chrono::steady_clock::now();
			float elapsedTime = std::chrono::duration<float>(now - lastAttackTime).count();

			if (elapsedTime >= attackCooldown)
			{
				lastAttackTime = now;
			}


		}



	};
}
