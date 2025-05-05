#include "../ecs/Component.h"
#include "../ecs/ecs.h"
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
#include "../ecs/ecs_defs.h"
#include "Transform.h"
#include <chrono>
#include "../map/DungeonFloor.h"
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
		DungeonFloor* floor;
		float speed;
	public : 
		__CMPID_DECL__(ecs::cmp::SLIMEMOVCMP);
		void init(DungeonFloor* dFloor) {
			floor = dFloor;
		}

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
					float dist = std::hypot(_slimeTransform->getPos().getX() - _player->getPos().getX(),
						_slimeTransform->getPos().getY() - _player->getPos().getY());
					if (dist > 50)
					{
						auto path = floor->findPathToX(_slimeTransform->getPos().getX() / 50, _slimeTransform->getPos().getY() / 50, _player->getPos().getX() / 50, _player->getPos().getY() / 50);
						//std::cout << Vector2D(path[1].x * 50, path[1].y * 50) << endl;

						if (path.size() > 0)
						{
							vector->CreateVector(_slimeTransform->getPos(), Vector2D(path[1].x * 50, path[1].y * 50));
							Vector2D velocity(vector->direcionX * speed, vector->direcionY * speed);
							_slimeTransform->getVel() = velocity;
						}
					}
					else
					{
						vector->CreateVector(_slimeTransform->getPos(), _player->getPos());

						Vector2D velocity(vector->direcionX * speed, vector->direcionY * speed);
						_slimeTransform->getVel() = velocity;
					}
				}
			

		}
	};  


}