#include "../ecs/Component.h"
#include "../ecs/ecs.h"
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
#include "../ecs/ecs_defs.h"
#include "Transform.h"
#include <chrono>
namespace ecs
{
	class StatComponent : public Component
	{

	public:
		__CMPID_DECL__(ecs::cmp::STATCMP);
		float life = 0;
		float speed = 0;
		float attackspeed = 0;
		float damage = 0;
		float attackrange = 0;
		virtual void harm(float damage)
		{
			life = life - damage;
			if (life <= 0)
			{
				Death();
			}
		}
		virtual void Death() 
		{
			delete _ent;
		};

		void initComponent() override
		{
			auto* _mngr = _ent->getMngr();
		}
		virtual void ChangeStat(string stat, float newStat)
		{
			if (stat == "speed")
			{
				speed += newStat;
			}
			else if (stat == "damage")
			{
				damage += newStat;
			}
			else if (stat == "attackSpeed")
			{
				attackspeed += newStat;
			}
			else if (stat == "attackRange")
			{
				attackrange += newStat;
			}

		}
		void update() override {}
	};

	class SlimeStatComponent : public StatComponent
	{
	public:
		__CMPID_DECL__(ecs::cmp::SLIMESTATCMP);
		void initComponent() override
		{
			auto* _mngr = _ent->getMngr();
			life = 10;
			speed = 0.5;
			attackspeed = 2;
			damage = 3;

		}

	};
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



	class SlimeMovementComponent : public Component
	{
		
		Transform* _slimeTransform;
		Transform* _player;
		SlimeStatComponent* stat;
		float speed;
	public : 
		__CMPID_DECL__(ecs::cmp::SLIMEMOVCMP);

		void initComponent() override
		{
			auto* _mngr = _ent->getMngr();
			_slimeTransform = _mngr->getComponent<Transform>(_ent);
			_player = _mngr->getComponent<Transform>(_mngr->getHandler(ecs::hdlr::PLAYER));
			stat = static_cast<SlimeStatComponent*>(_ent->getMngr()->getComponent<SlimeStatComponent>(_ent));
			speed = stat->speed;
		}

		void update() override

		{			
			auto vector = static_cast<SlimeVectorComponent*>(_ent->getMngr()->getComponent<SlimeVectorComponent>(_ent));
				

				if (vector && stat && _slimeTransform)
				{
					vector->CreateVector(_slimeTransform->getPos(), _player->getPos());
					
					Vector2D velocity (vector->direcionX * speed, vector->direcionY * speed);
					_slimeTransform->getVel() = velocity;
				}
			

		}
	};  

	class SlimeAttackComponent : public Component
	{
		
		Transform* _slimeTransform;
		Transform* _player;
		SlimeStatComponent* stat;
		bool atack = false;
		float height, width;
		float attackspeed = 10 ;

	public:
		__CMPID_DECL__(ecs::cmp::SLIMEATKCMP);
	
		std::chrono::steady_clock::time_point lastAttackTime = std::chrono::steady_clock::now();

		void initComponent() override
		{
			auto* _mngr = _ent->getMngr();
			_slimeTransform = _mngr->getComponent<Transform>(_ent);
			_player = _mngr->getComponent<Transform>(_mngr->getHandler(ecs::hdlr::PLAYER));
			 //stat = static_cast<SlimeStatComponent*>(_ent->getMngr()->getComponent<SlimeStatComponent>(_ent));
			stat = _mngr->getComponent<SlimeStatComponent>(_ent);
			attackspeed -= stat->attackspeed;
		}

		void update() override
		{


	
			
			auto now = std::chrono::steady_clock::now();
			float elapsedTime = std::chrono::duration<float>(now - lastAttackTime).count();

			if (elapsedTime >= attackspeed)
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
			

		}




	};
}