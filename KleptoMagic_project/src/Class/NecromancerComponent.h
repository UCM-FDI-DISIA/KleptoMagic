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
	class NecroStatComponent : public Component
	{
		Transform* _NecroTransform;
		Transform* _player;
	public:
		__CMPID_DECL__(ecs::cmp::NECROSTATCMP);
		float life = 10;
		float shield = 0.60;
		void initComponent() override
		{
			auto* _mngr = _ent->getMngr();
			_NecroTransform = _mngr->getComponent<Transform>(_ent);
			_player = _mngr->getComponent<Transform>(_mngr->getHandler(ecs::hdlr::PLAYER));
		}
		void harm(float damage) 
		{
			life = life - (damage * shield);
			 if (life <= 0)
			 {

			 }
		}
		void Death() {};
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
			enemyutils().spawn_enemy(ENEMY_SLIME, Vector2D (_NecroTransform->getPos().getX() + 10, _NecroTransform->getPos().getY()));
			enemyutils().spawn_enemy(ENEMY_SLIME, Vector2D(_NecroTransform->getPos().getX() - 10, _NecroTransform->getPos().getY()));
			enemyutils().spawn_enemy(ENEMY_SLIME, Vector2D(_NecroTransform->getPos().getX(), _NecroTransform->getPos().getY() + 10));
		}
		void update () 
		{
			
			if (spawned = 0 )
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

		void Death()
		{
			NecroMancer->getMngr()->getComponent<NecroSpawnerComponent>(NecroMancer)->spawnKilled();
		}
	};
	class NecroMovementComponent: public Component 
	{
		Transform* _NecroTransform;
		Transform* _player;
		

	public:
		__CMPID_DECL__(ecs::cmp::NECROMOVCMP);
		void initComponent() override
		{
			auto* _mngr = _ent->getMngr();
			_NecroTransform = _mngr->getComponent<Transform>(_ent);
			_player = _mngr->getComponent<Transform>(_mngr->getHandler(ecs::hdlr::PLAYER));
		}

		void update() override

		{
			auto vector = static_cast<NecroVectorComponent*>(_ent->getMngr()->getComponent<NecroVectorComponent>(_ent));
			
			if (vector  && _NecroTransform)
			{
				vector->CreateVector(_NecroTransform->getPos(), _player->getPos());
				if (vector->magnitude < 10)
				{
					Vector2D velocity(vector->direcionX * 0.5, vector->direcionY * 0.5);
					_NecroTransform->getVel() = velocity;
				}
				
			}


		}
	};
}
