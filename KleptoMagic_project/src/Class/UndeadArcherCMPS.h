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


	class UndeadStatComponent : public Component
	{
		Transform* _UndeadTransform;
		Transform* _player;
	public:
		__CMPID_DECL__(ecs::cmp::UNDEADSTATCMP);
		float life = 50;

		void initComponent() override
		{
			auto* _mngr = _ent->getMngr();
			_UndeadTransform = _mngr->getComponent<Transform>(_ent);
			_player = _mngr->getComponent<Transform>(_mngr->getHandler(ecs::hdlr::PLAYER));
		}
		void harm(float damage)
		{
			life = life - damage;
			if (life <= 0)
			{

			}
		}
		void Death() {};
		void update() override {}
	};

	class UndeadMovementComponent : public Component
	{

		Transform* _UndeadTransform;
		Transform* _player;
	public:
		__CMPID_DECL__(ecs::cmp::UNDEADMOVCMP);

		void initComponent() override
		{
			auto* _mngr = _ent->getMngr();
			_UndeadTransform = _mngr->getComponent<Transform>(_ent);
			_player = _mngr->getComponent<Transform>(_mngr->getHandler(ecs::hdlr::PLAYER));
		}

		void Move()

		{
			auto vector = static_cast<UndeadVectorComponent*>(_ent->getMngr()->getComponent<UndeadVectorComponent>(_ent));
			auto stat = static_cast<UndeadStatComponent*>(_ent->getMngr()->getComponent<UndeadStatComponent>(_ent));

			if (vector && stat && _UndeadTransform)
			{
				vector->CreateVector(_player->getPos(), _UndeadTransform->getPos());
				Vector2D velocity(vector->direcionX * 0.5, vector->direcionY * 0.5);
				_UndeadTransform->getVel() = velocity;
			}


		}
	};

	class UndeadAttackComponent : public Component
	{
	public:
		Transform* _UndeadTransform;
		Transform* _player;
		std::chrono::steady_clock::time_point lastAttackTime = std::chrono::steady_clock::now();
		double attackRange;
		__CMPID_DECL__(ecs::cmp::UNDEADATKCMP);
		void initComponent() override
		{
			auto* _mngr = _ent->getMngr();
			_UndeadTransform = _mngr->getComponent<Transform>(_ent);
			_player = _mngr->getComponent<Transform>(_mngr->getHandler(ecs::hdlr::PLAYER));
		}
		void update() override
		{
			
				auto vector = static_cast<UndeadVectorComponent*>(_ent->getMngr()->getComponent<UndeadVectorComponent>(_ent));
				auto stat = static_cast<UndeadStatComponent*>(_ent->getMngr()->getComponent<UndeadStatComponent>(_ent));
				auto movement = static_cast<UndeadMovementComponent*>(_ent->getMngr()->getComponent<UndeadMovementComponent>(_ent));
		
				auto now = std::chrono::steady_clock::now();
				float elapsedTime = std::chrono::duration<float>(now - lastAttackTime).count();

				vector->CreateVector(_player->getPos(), _UndeadTransform->getPos());
				Vector2D attackdirection(vector->direcionX * 1, vector->direcionY *1);
				attackRange = vector->magnitude;

				if (elapsedTime >= 10 && attackRange <= 200)
				{
					//create bullet
					
					lastAttackTime = now;
					_UndeadTransform->getVel() =  _UndeadTransform->getVel() * 0;
				}
				if (attackRange > 200) 
				{
					movement->Move();
				}			 
			
		}

	};
}
