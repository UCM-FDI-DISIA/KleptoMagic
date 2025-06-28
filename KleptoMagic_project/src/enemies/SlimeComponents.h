#include "../ecs/Component.h"
#include "../ecs/ecs.h"
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
#include "../ecs/ecs_defs.h"
#include "../ecs/Transform.h"
#include <chrono>
#include "../map/DungeonFloor.h"
#include "../render/AnimatorComponent.h"
#include "../ecs/EntityStat.h"

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
			speed = 2;
			attackspeed = 2;
			damage = 1;
			attackrange = 25;
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

	class SlimeAttackComponent : public Component
	{	
		Transform* _slimeTransform;
		Transform* _player;
		SlimeStatComponent* stat;
		bool atack = false;
		float height, width;
		float attackspeed = 6;
		float attackRange;

	public:
		__CMPID_DECL__(ecs::cmp::SLIMEATKCMP);
	
		std::chrono::steady_clock::time_point lastAttackTime = std::chrono::steady_clock::now();

		void initComponent() override
		{
			auto* _mngr = _ent->getMngr();
			_slimeTransform = _mngr->getComponent<Transform>(_ent);
			_player = _mngr->getComponent<Transform>(_mngr->getHandler(ecs::hdlr::PLAYER));
			stat = _mngr->getComponent<SlimeStatComponent>(_ent);
			attackspeed -= stat->attackspeed;
			attackRange = stat->attackrange;
		}

		void update() override
		{
			auto now = std::chrono::steady_clock::now();
			float elapsedTime = std::chrono::duration<float>(now - lastAttackTime).count();

			// Calculamos distancia al jugador
			float distanceToPlayer = std::hypot(
				_slimeTransform->getPos().getX() - _player->getPos().getX(),
				_slimeTransform->getPos().getY() - _player->getPos().getY()
			);

			// Solo atacar si está dentro del rango
			if (distanceToPlayer <= attackRange && elapsedTime >= attackspeed)
			{
				height = _slimeTransform->getHeight();
				width = _slimeTransform->getWidth();

				_slimeTransform->setHeight(height * 1.5f);
				_slimeTransform->setWidth(width * 1.5f);

				lastAttackTime = now;
				atack = true;

				// DAÑO AL JUGADOR
				auto* _mngr = _ent->getMngr();
				auto player = _mngr->getHandler(ecs::hdlr::PLAYER);
				if (_mngr->isAlive(player)) {
					auto* playerStats = _mngr->getComponent<EntityStat>(player);
					if (playerStats != nullptr) {
						playerStats->ChangeStat(-stat->damage, EntityStat::Stat::HealthCurrent);
					}
				}
			}
			else if (elapsedTime >= 0.2f && atack)
			{
				_slimeTransform->setHeight(height);
				_slimeTransform->setWidth(width);
				atack = false;
			}
		}
	};

	class SlimeAnimComponent : public AnimatorComponent {
		friend AnimatorComponent;
	public:
		__CMPID_DECL__(ecs::cmp::SLIMEANIMCMP);

		SlimeAnimComponent() {
			isWalking = false;
			isFacingRight = false;
		}

		void update() override {
			// check speed to see if it's walking or not
			float velX = _tr->getVel().getX();
			float velY = _tr->getVel().getY();
			bool isCurrentlyWalking = (velX != 0 || velY != 0);
			bool isCurrentlyMovingSideways = (velX != 0);
			bool isCurrentlyMovingRight = (velX < 0);
			//cout << velX << "|" << velY << "   " << isCurrentlyWalking << "|" << isWalking << "   " << isCurrentlyMovingRight << "|" <<isFacingRight << endl;

			if (!isWalking && isCurrentlyWalking) {
				toggleWalkingAnim();
			}
			else if (isWalking && !isCurrentlyWalking) {
				toggleWalkingAnim();
			}

			if (!isFacingRight && !isCurrentlyMovingRight && isCurrentlyMovingSideways) {
				toggleFlip();
			}
			else if (isFacingRight && isCurrentlyMovingRight && isCurrentlyMovingSideways) {
				toggleFlip();
			}
		}

		void toggleWalkingAnim() {
			if (!isWalking) {
				_img->setStartingFrame(startFrame);
				_img->setFrame(startFrame + 1);
				_img->setNumFrames(2);
			}
			else {
				_img->setStartingFrame(startFrame);
				_img->setFrame(startFrame);
				_img->setNumFrames(1);
			}
			isWalking = !isWalking;
		}

	private:
		void createStart();
	};
}