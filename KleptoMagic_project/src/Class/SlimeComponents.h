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
			direcionX = enemyPos.getX() - playerPos.getX() ;
			direcionY =  enemyPos.getY() - playerPos.getY();
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
		__CMPID_DECL__(ecs::cmp::SLIMESTATCMP);
		float life = 13;
		void harm(float damage)
		{
			life = life - damage;
			if (life <= 0)
			{

			}
		}
		void Death() {};

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
				auto vector = static_cast<SlimeVectorComponent*>(_ent->getMngr()->getComponent<SlimeVectorComponent>(_ent));
				auto stat = static_cast<SlimeStatComponent*>(_ent->getMngr()->getComponent<SlimeStatComponent>(_ent));

				if (vector && stat && _slimeTransform)
				{
					vector->CreateVector(_slimeTransform->getPos(), _player->getPos());
					Vector2D velocity (vector->direcionX * 0.5, vector->direcionY * 0.5);
					_slimeTransform->getVel() = velocity;
				}
			

		}
	};  

	class SlimeAttackComponent : public Component
	{
		
		Transform* _slimeTransform;
		Transform* _player;
		bool colision = false;
		bool atack = false;
		float height, width;
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
		void Colision()
		{
			colision = true;
		}
		void update() override
		{


			auto stat = static_cast<SlimeStatComponent*>(_ent->getMngr()->getComponent<SlimeStatComponent>(_ent));
			attackCooldown = 10;
			auto now = std::chrono::steady_clock::now();
			float elapsedTime = std::chrono::duration<float>(now - lastAttackTime).count();

			if (elapsedTime >= attackCooldown)
			{
				height = _slimeTransform->getHeight();
				width = _slimeTransform->getWidth();

				_slimeTransform->setHeight(height * 1.5);
				_slimeTransform->setWidth(width * 1.5);

				lastAttackTime = now;
				atack = true;
			}
			else if (elapsedTime >= 0.5 && atack)
			{
				_slimeTransform->setHeight(height);
				_slimeTransform->setWidth(width);
				atack = false;
			}
			colision = false;

		}




	};
}