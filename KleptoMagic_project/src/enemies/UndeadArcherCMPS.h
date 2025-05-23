#include "../ecs/Component.h"
#include "../ecs/ecs.h"
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
#include "../ecs/ecs_defs_example.h"
#include "../ecs/Transform.h"
#include "../sdlutils/Texture.h"
#include <chrono>

namespace ecs
{

	class UndeadStatComponent : public StatComponent
	{
	public:
		__CMPID_DECL__(ecs::cmp::UNDEADSTATCMP);
		void initComponent() override
		{
			auto* _mngr = _ent->getMngr();
			life = 8;
			speed = 1;
			attackspeed = 10;
			damage = 4;
			attackrange = 10;

		}

	};

	class UndeadVectorComponent : public Component
	{
	public:
		__CMPID_DECL__(ecs::cmp::UNDEADVECCMP);
		float direcionX, direcionY;
		double magnitude;
		Transform* _UndeadTransform;
		Transform* _player;

		void initComponent() override
		{
			auto* _mngr = _ent->getMngr();
			_UndeadTransform = _mngr->getComponent<Transform>(_ent);
			_player = _mngr->getComponent<Transform>(_mngr->getHandler(ecs::hdlr::PLAYER));
		}
		void CreateVector(Vector2D playerPos, Vector2D enemyPos) {
			// Calculamos el vector direcci�n
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


	

	class UndeadMovementComponent : public Component
	{

		Transform* _UndeadTransform;
		Transform* _player;
		float speed;
		DungeonFloor* floor;

	public:
		__CMPID_DECL__(ecs::cmp::UNDEADMOVCMP);

		void init(DungeonFloor* dFloor) {
			floor = dFloor;
		}

		void initComponent() override
		{
			auto* _mngr = _ent->getMngr();
			_UndeadTransform = _mngr->getComponent<Transform>(_ent);
			_player = _mngr->getComponent<Transform>(_mngr->getHandler(ecs::hdlr::PLAYER));
			auto stat = static_cast<UndeadStatComponent*>(_ent->getMngr()->getComponent<UndeadStatComponent>(_ent));
			speed = stat->speed;
		}

		void Move()

		{
			auto vector = static_cast<UndeadVectorComponent*>(_ent->getMngr()->getComponent<UndeadVectorComponent>(_ent));
			

			if (vector && stat && _UndeadTransform)
			{
				float dist = std::hypot(_UndeadTransform->getPos().getX() - _player->getPos().getX(),
					_UndeadTransform->getPos().getY() - _player->getPos().getY());
				if (dist > 50)

				{
					auto path = floor->findPathToX(_UndeadTransform->getPos().getX() / 50, _UndeadTransform->getPos().getY() / 50, _player->getPos().getX() / 50, _player->getPos().getY() / 50);
					//std::cout << Vector2D(path[1].x * 50, path[1].y * 50) << endl;

					if (path.size() > 0)
					{
						vector->CreateVector(Vector2D(path[1].x * 50, path[1].y * 50), _UndeadTransform->getPos());
						Vector2D velocity(vector->direcionX * speed, vector->direcionY * speed);
						_UndeadTransform->getVel() = velocity;
					}
				}
				else
				{
					vector->CreateVector(_player->getPos(), _UndeadTransform->getPos());
					Vector2D velocity(vector->direcionX * speed, vector->direcionY * speed);
					_UndeadTransform->getVel() = velocity;
				}
			}


		}
	};

	class UndeadAttackComponent : public Component
	{
	public:
		Transform* _UndeadTransform;
		Transform* _player;
		std::chrono::steady_clock::time_point lastAttackTime = std::chrono::steady_clock::now();
		float attackRange;
		float attackspeed;
		float range;
		__CMPID_DECL__(ecs::cmp::UNDEADATKCMP);
		void initComponent() override
		{
			auto* _mngr = _ent->getMngr();
			_UndeadTransform = _mngr->getComponent<Transform>(_ent);
			_player = _mngr->getComponent<Transform>(_mngr->getHandler(ecs::hdlr::PLAYER));
			auto stat = static_cast<UndeadStatComponent*>(_ent->getMngr()->getComponent<UndeadStatComponent>(_ent));
			range = stat->attackrange;
			attackspeed = stat->attackspeed;
		}
		void update() override
		{
			
				auto vector = static_cast<UndeadVectorComponent*>(_ent->getMngr()->getComponent<UndeadVectorComponent>(_ent));
			
				auto movement = static_cast<UndeadMovementComponent*>(_ent->getMngr()->getComponent<UndeadMovementComponent>(_ent));
		
				auto now = std::chrono::steady_clock::now();
				float elapsedTime = std::chrono::duration<float>(now - lastAttackTime).count();

				vector->CreateVector(_player->getPos(), _UndeadTransform->getPos());
				Vector2D attackdirection(vector->direcionX , vector->direcionY);
				attackRange = vector->magnitude;

				if (elapsedTime >= attackspeed && attackRange <= range)
				{
					//create bullet
					
					lastAttackTime = now;
					_UndeadTransform->getVel() =  _UndeadTransform->getVel() * 0;
				}
				if (attackRange > range) 
				{
					movement->Move();
				}			 
			
		}

	};
}
