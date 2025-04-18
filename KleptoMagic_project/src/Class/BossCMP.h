#include "../ecs/Component.h"
#include "../ecs/ecs.h"
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
#include "../ecs/ecs_defs_example.h"
#include "Transform.h"
#include "../sdlutils/Texture.h"
#include <chrono>
namespace ecs
{
	class BossVectorComponent : public Component
	{
	public:
		__CMPID_DECL__(ecs::cmp::BOSSVECCMP);
		float direcionX, direcionY;
		double magnitude;
		Transform* _BossTransform;
		Transform* _player;

		void initComponent() override
		{
			auto* _mngr = _ent->getMngr();
			_BossTransform = _mngr->getComponent<Transform>(_ent);
			_player = _mngr->getComponent<Transform>(_mngr->getHandler(ecs::hdlr::PLAYER));
		}
		void CreateVector(Vector2D playerPos, Vector2D enemyPos) {
			// Calculamos el vector dirección
			direcionX = playerPos.getX() - enemyPos.getX();
			direcionY = playerPos.getY() - enemyPos.getY();

			// Calculamos la magnitud del vector ANTES de normalizarlo
			magnitude = std::sqrt(direcionX * direcionX + direcionY * direcionY);

			// Evitamos divisiones por 0
			if (magnitude > 0.0001) {
				direcionX /= magnitude;  // Normalizamos X
				direcionY /= magnitude;  // Normalizamos Y
			}
		}
	};


	class BossStatComponent : public Component
	{
		Transform* _BossTransform;
		Transform* _player;
	public:
		__CMPID_DECL__(ecs::cmp::BOSSSTATCMP);
		float speed = 50;
		float damage = 10;
		float attackspeed = 1;
		float range = 5;
		void initComponent() override
		{
			auto* _mngr = _ent->getMngr();
			_BossTransform = _mngr->getComponent<Transform>(_ent);
			_player = _mngr->getComponent<Transform>(_mngr->getHandler(ecs::hdlr::PLAYER));
		}
		void update() override {}
	};

	class BossMovementComponent : public Component
	{

		Transform* _BossTransform;
		Transform* _player;
	public:
		__CMPID_DECL__(ecs::cmp::BOSSMOVCMP);

		void initComponent() override
		{
			auto* _mngr = _ent->getMngr();
			_BossTransform = _mngr->getComponent<Transform>(_ent);
			_player = _mngr->getComponent<Transform>(_mngr->getHandler(ecs::hdlr::PLAYER));
		}

		void Move()

		{
			auto vector = static_cast<BossVectorComponent*>(_ent->getMngr()->getComponent<BossVectorComponent>(_ent));
			auto stat = static_cast<BossStatComponent*>(_ent->getMngr()->getComponent<BossStatComponent>(_ent));

			if (vector && stat && _BossTransform)
			{
				vector->CreateVector(_player->getPos(), _BossTransform->getPos());
				Vector2D velocity(vector->direcionX * 0.5, vector->direcionY * 0.5);
				_BossTransform->getVel() = velocity;
			}


		}
	};

	class BossAttackComponent : public Component
	{
	public:
		Transform* _BossTransform;
		Transform* _player;
		Entity* player = nullptr;
		float attackCooldown;
		std::chrono::steady_clock::time_point lastAttackTime = std::chrono::steady_clock::now();
		double attackRange;
		__CMPID_DECL__(ecs::cmp::BOSSATKCMP);
		void initComponent() override
		{
			auto* _mngr = _ent->getMngr();
			_BossTransform = _mngr->getComponent<Transform>(_ent);
			_player = _mngr->getComponent<Transform>(_mngr->getHandler(ecs::hdlr::PLAYER));
		}
		void update() override
		{

			auto vector = static_cast<BossVectorComponent*>(_ent->getMngr()->getComponent<BossVectorComponent>(_ent));
			auto stat = static_cast<BossStatComponent*>(_ent->getMngr()->getComponent<BossStatComponent>(_ent));
			auto movement = static_cast<BossMovementComponent*>(_ent->getMngr()->getComponent<BossMovementComponent>(_ent));
			attackCooldown = 10;
			auto now = std::chrono::steady_clock::now();
			float elapsedTime = std::chrono::duration<float>(now - lastAttackTime).count();

			vector->CreateVector(_player->getPos(), _BossTransform->getPos());
			Vector2D attackdirection(vector->direcionX * 1, vector->direcionY * 1);
			attackRange = vector->magnitude;

			if (elapsedTime >= 10 && attackRange <= 200)
			{
				//elige un ataque de los diferentes ataques que tiene, puede ser por porcentaje o por random ya veremos

				lastAttackTime = now;
				_BossTransform->getVel() = _BossTransform->getVel() * 0;
			}
			if (attackRange > 200)
			{
				movement->Move();
			}

		}

		// Diferentes ataques con patrones diferentes

	};
}
