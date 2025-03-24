#include "../ecs/Component.h"
#include "../ecs/ecs.h"
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
#include "../ecs/ecs_defs.h"
#include "Transform.h"
#include <chrono>
namespace ecs
{
	class SlimeVectorComponent : public Component
	{
	private:
		void createStart();
		
		Transform* _slimeTransform;
		Transform* _playerTransform;
	public:
		__CMPID_DECL__(ecs::cmp::SLIMEVECCMP);
		float direcionX, direcionY;
	
		void initComponent() override
		{
			auto* _mngr = _ent->getMngr();
		}
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
		
		Transform* _ghostTransform;
		Transform* _playerTransform;
	public:
		__CMPID_DECL__(ecs::cmp::SLIMESTATCMP);
		float speed = 50;
		float damage = 10;
		float attackspeed = 10;

		void initComponent() override
		{
			auto* _mngr = _ent->getMngr();
		}
		void update() override {}
	};



	class SlimeMovementComponent : public Component
	{
		
		Transform* _slimeTransform;
		Transform* _player;
		Entity* player = nullptr;
	public : 
		__CMPID_DECL__(ecs::cmp::SLIMEMOVCMP);

		void initComponent() override
		{
			auto* _mngr = _ent->getMngr();
			_slimeTransform = _mngr->getComponent<Transform>(_ent);
			_player = _mngr->getComponent<Transform>(_mngr->getHandler(ecs::hdlr::PLAYER));

		}

		void update() override

		{
			if (player != nullptr)
			{
				auto vector = static_cast<SlimeVectorComponent*>(_ent->getMngr()->getComponent<SlimeVectorComponent>(_ent));
				auto stat = static_cast<SlimeStatComponent*>(_ent->getMngr()->getComponent<SlimeStatComponent>(_ent));

				if (vector && stat && _slimeTransform)
				{
					vector->CreateVector(_slimeTransform->getPos(), _player->getPos());
					Vector2D velocity (vector->direcionX * stat->speed, vector->direcionY * stat->speed);
					_slimeTransform->getVel() = velocity;
				}
			}

		}
	};

	class SlimeAttackComponent : public Component
	{
		
		Transform* _slimeTransform;
		Transform* _player;
		Entity* player = nullptr;
	public:
		__CMPID_DECL__(ecs::cmp::SLIMESTATCMP);
		float attackCooldown;
		std::chrono::steady_clock::time_point lastAttackTime = std::chrono::steady_clock::now();

		void initComponent() override
		{
			auto* _mngr = _ent->getMngr();
			_slimeTransform = _mngr->getComponent<Transform>(_ent);
			_player = _mngr->getComponent<Transform>(_mngr->getHandler(ecs::hdlr::PLAYER));
		}
		void update() override
		{


			auto stat = static_cast<SlimeStatComponent*>(_ent->getMngr()->getComponent<SlimeStatComponent>(_ent));
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