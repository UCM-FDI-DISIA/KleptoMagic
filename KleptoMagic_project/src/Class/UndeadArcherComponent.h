#include "../ecs/Component.h"
#include "../ecs/ecs.h"
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
#include "../ecs/ecs_defs_example.h"
#include "Transform.h"
#include <chrono>
namespace ecs
{
	class UndeadArcherVectorComponent : public Component
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

	class CollideComponent : public Component
	{

	};
	class RenderComponent : public Component
	{

	};

	class UndeadArcherStatComponent: public Component
	{
	public:
		float speed = 50;
		float damage = 10;
		float attackspeed = 10;
		float range = 10;

		void update() override {}
	};
	class UndeadAttackComponent : public Component
	{
	public:
		Entity* player = nullptr;
		float attackCooldown;
		std::chrono::steady_clock::time_point lastAttackTime = std::chrono::steady_clock::now();
		float attackRange;
		void update() override
		{
			if (player != nullptr)
			{
				auto vector = static_cast<UndeadArcherVectorComponent*>(_ent->getMngr()->getComponent<UndeadArcherVectorComponent>(_ent));
				auto stat = static_cast<UndeadArcherStatComponent*>(_ent->getMngr()->getComponent<UndeadArcherStatComponent>(_ent));
				auto _transform = static_cast<Transform*>(_ent->getMngr()->getComponent<Transform>(_ent));

				auto _player = static_cast<Transform*>(_ent->getMngr()->getComponent<Transform>(player));



				attackCooldown = 10 - stat->attackspeed;
				auto now = std::chrono::steady_clock::now();
				float elapsedTime = std::chrono::duration<float>(now - lastAttackTime).count();

				vector->CreateVector(_transform->getPos(), _player->getPos());
				Vector2D attackspeed(vector->direcionX, vector->direcionY);
				attackRange = attackspeed.magnitude();

				if (elapsedTime >= attackCooldown && attackRange <= stat->range);
				{
					//create bullet
					lastAttackTime = now;
				}

			}
		}



	};
}
