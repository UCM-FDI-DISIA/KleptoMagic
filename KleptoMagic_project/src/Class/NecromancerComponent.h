#include "../ecs/Component.h"
#include "../ecs/ecs.h"
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
#include "../ecs/ecs_defs_example.h"
#include "Transform.h"
#include "../sdlutils/Texture.h"
#include "../game/EnemyUtils.h"

#include <chrono>
namespace ecs 
{
	class NecroStatComponent : public StatComponent
	{
		Transform* _NecroTransform;
		Transform* _player;
	public:
		__CMPID_DECL__(ecs::cmp::NECROSTATCMP);

		float shield = 0.60;
		void initComponent() override
		{
			auto* _mngr = _ent->getMngr();
			life = 11;
			speed = 1.3;
		}
		void harm(float damage) override
		{
			life = life - (damage * shield);
			if (life <= 0)
			{
				Death();
			}
		};
		void ReduceShield()
		{
			if (shield >= 1)
			{
				shield = 1;
			}
			else
			{
				shield += 0.1;
			}
		}
		void update() override {}
	};
	class NecroVectorComponent : public Component
	{
	public:
		float direcionX, direcionY;
		double magnitude;
		Transform* _NecroTransform;
		Transform* _player;
		__CMPID_DECL__(ecs::cmp::NECROVECCMP);

		void initComponent() override
		{
			auto* _mngr = _ent->getMngr();
			_NecroTransform = _mngr->getComponent<Transform>(_ent);
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
	
	class NecroSpawnerComponent: public Component 
	{
		Transform* _NecroTransform;
		Transform* _player;
		float spawned = 0;
		
		

	public:
		__CMPID_DECL__(ecs::cmp::NECROCRTCMP);
		std::chrono::steady_clock::time_point lastAttackTime = std::chrono::steady_clock::now();
		void initComponent() override
		{
			auto* _mngr = _ent->getMngr();
			_NecroTransform = _mngr->getComponent<Transform>(_ent);
			_player = _mngr->getComponent<Transform>(_mngr->getHandler(ecs::hdlr::PLAYER));
			
		}
		void Spawn()
		{
			enemyutils().necro_spawn(_ent, 10, 0);
			enemyutils().necro_spawn(_ent, -10, 0);
			enemyutils().necro_spawn(_ent, 0, 10);
		}
		void update () 
		{
			
			if (spawned == 0 )
			{
				Spawn();
				spawned = 3;
			}
		}
		void spawnKilled() 
		{
			spawned--;
			auto shield = static_cast<NecroStatComponent*>(_ent->getMngr()->getComponent<NecroStatComponent>(_ent));
			shield->ReduceShield();
		}
	};
	class SpawnComponent : public Component 
	{
		
		Transform* _SpawnTransform;
		Transform* _player;
		Entity* NecroMancer;
		float spawned = 0;
		

	public:
		__CMPID_DECL__(ecs::cmp::SPAWNCMP);
		void initComponent() override
		{
			auto* _mngr = _ent->getMngr();
			_SpawnTransform = _mngr->getComponent<Transform>(_ent);
			_player = _mngr->getComponent<Transform>(_mngr->getHandler(ecs::hdlr::PLAYER));
			
		}
		void setParent(Entity* father) { NecroMancer = father; }

		void Dead()
		{
			
			NecroMancer->getMngr()->getComponent<NecroSpawnerComponent>(NecroMancer)->spawnKilled();
		}
	};
	class SpawnedStatComponent: public StatComponent 
	{
	public:
		__CMPID_DECL__(ecs::cmp::SPAWNSTATCMP);
		void initComponent() override
		{
			auto* _mngr = _ent->getMngr();
			life = 10;
			speed = 0.5;
			attackspeed = 2;
			damage = 3;

		}
		void Death() override
		{
			auto dead = static_cast<SpawnComponent*>(_ent->getMngr()->getComponent<SpawnComponent>(_ent));
			dead->Dead();
			delete _ent;
		}

	};
	class SpawnMovementComponent: public Component 
	{

		Transform* _slimeTransform;
		Transform* _player;
		SpawnedStatComponent* stat;
		float speed;
	public:
		__CMPID_DECL__(ecs::cmp::SPAWNMOVCMP);

		void initComponent() override
		{
			auto* _mngr = _ent->getMngr();
			_slimeTransform = _mngr->getComponent<Transform>(_ent);
			_player = _mngr->getComponent<Transform>(_mngr->getHandler(ecs::hdlr::PLAYER));
			stat = static_cast<SpawnedStatComponent*>(_ent->getMngr()->getComponent<SpawnedStatComponent>(_ent));
			speed = stat->speed;
		}

		void update() override

		{
			auto vector = static_cast<SlimeVectorComponent*>(_ent->getMngr()->getComponent<SlimeVectorComponent>(_ent));


			if (vector && stat && _slimeTransform)
			{
				vector->CreateVector(_slimeTransform->getPos(), _player->getPos());

				Vector2D velocity(vector->direcionX * speed, vector->direcionY * speed);
				_slimeTransform->getVel() = velocity;
			}


		}
	};

	class NecroMovementComponent: public Component 
	{
		Transform* _NecroTransform;
		Transform* _player;
		float speed;
		

	public:
		__CMPID_DECL__(ecs::cmp::NECROMOVCMP);
		void initComponent() override
		{
			auto* _mngr = _ent->getMngr();
			_NecroTransform = _mngr->getComponent<Transform>(_ent);
			_player = _mngr->getComponent<Transform>(_mngr->getHandler(ecs::hdlr::PLAYER));
			auto stat = static_cast<NecroStatComponent*>(_ent->getMngr()->getComponent<NecroStatComponent>(_ent));
			speed = stat->speed;
		}

		void update() override

		{
			auto vector = static_cast<NecroVectorComponent*>(_ent->getMngr()->getComponent<NecroVectorComponent>(_ent));
			
			if (vector  && _NecroTransform)
			{
				vector->CreateVector(_NecroTransform->getPos(), _player->getPos());
				if (vector->magnitude < 10)
				{
					Vector2D velocity(vector->direcionX * speed, vector->direcionY * speed);
					_NecroTransform->getVel() = velocity;
				}
				
			}


		}
	};
}
