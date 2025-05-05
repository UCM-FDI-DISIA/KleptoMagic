#include "../ecs/Component.h"
#include "../ecs/ecs.h"
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
#include "../ecs/ecs_defs_example.h"
#include "Transform.h"
#include "../sdlutils/Texture.h"
#include "../map/DungeonFloor.h"
#include "BulletUtils.h"
#include <chrono>
#include <thread>   
#include <random>

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
		int random;
		bool canMove = false;
		Vector2D direction;
		UndeadBehaviourComponent* behaviour;
		DungeonFloor* _dungeonfloor;

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
			behaviour = static_cast<UndeadBehaviourComponent*>(_ent->getMngr()->getComponent<UndeadBehaviourComponent>(_ent));
		}
		void init(DungeonFloor* floor)
		{
			_dungeonfloor = floor;
		}
		void ChooseRandom()
		{
			std::random_device rd;  // Semilla aleatoria
			std::mt19937 gen(rd()); // Motor Mersenne Twister
			std::uniform_int_distribution<> distrib(0, 3); // Rango [0, 3]

			random = distrib(gen);
		}

		void Move()

		{
			
			if (random == 0) 
			{
				direction = { 1,0 };
			}
			else if (random == 1)
			{
				direction = { -1,0 };
			}
			else if (random == 2)
			{
				direction = { 0,1 };
			}
			else if (random == 3) 
			{
				direction = { 0,-1 };
			}
				
		Vector2D velocity(direction.getX() * speed, direction.getY() * speed);
		int resultX = _dungeonfloor->checkCollisions(_UndeadTransform->getPos().getX() + velocity.getX(), _UndeadTransform->getPos().getY() + velocity.getY());
		 if (resultX == 0)
		 {
					_UndeadTransform->getVel() = velocity;
		 }
		 else
		 {
					_UndeadTransform->getVel() = {0,0};
		 }							
		}
	
	};

	class UndeadAttackComponent : public Component
	{
	public:
		Transform* _UndeadTransform;
		Transform* _player;
		Vector2D aim;
		std::chrono::steady_clock::time_point lastAttackTime = std::chrono::steady_clock::now();
		DungeonFloor* _dungeonfloor;
		BulletUtils* bullet;
		__CMPID_DECL__(ecs::cmp::UNDEADATKCMP);
		void initComponent() override
		{
			auto* _mngr = _ent->getMngr();
			_UndeadTransform = _mngr->getComponent<Transform>(_ent);
			_player = _mngr->getComponent<Transform>(_mngr->getHandler(ecs::hdlr::PLAYER));
		}
		void init(DungeonFloor* floor, BulletUtils* bull)
		{
			_dungeonfloor = floor;
			bullet = bull;
		}
		void Shoot()
		{
			
			bullet->enemyShoot(_UndeadTransform, 1);
		}
		void GetVector(Vector2D aim)
		{
			this->aim = aim;
		}
	};
	class UndeadBehaviourComponent : public Component {
		Transform* _UndeadTransform = nullptr;
		Transform* _player = nullptr;
		UndeadVectorComponent* vector = nullptr;
		UndeadMovementComponent* movement = nullptr;
		DungeonFloor* _dungeonfloor;
		float attackspeed = 1.0f; // segundos entre ataques
		bool isMoving = true;
		float speed;
		int resultX = 0;
		std::chrono::steady_clock::time_point lastActionTime = std::chrono::steady_clock::now();

	public:
		__CMPID_DECL__(ecs::cmp::UNDEADBEHACMP);

		void initComponent() override {
			auto* _mngr = _ent->getMngr();
			_UndeadTransform = _mngr->getComponent<Transform>(_ent);
			_player = _mngr->getComponent<Transform>(_mngr->getHandler(ecs::hdlr::PLAYER));
			vector = _mngr->getComponent<UndeadVectorComponent>(_ent);
			movement = _mngr->getComponent<UndeadMovementComponent>(_ent);
			
			auto stat = _mngr->getComponent<UndeadStatComponent>(_ent);
			if (stat) {
				attackspeed = stat->attackspeed;
				speed = stat->speed;
			}

			if (movement) movement->ChooseRandom();
		}
		void init(DungeonFloor* floor)
		{
			_dungeonfloor = floor;
		}

		void update() override {
			if (!_UndeadTransform || !_player || !vector || !movement) return;
			bool chocan = false;
			auto now = std::chrono::steady_clock::now();
			float elapsed = std::chrono::duration<float>(now - lastActionTime).count();
			int i = 0;
			vector->CreateVector(_player->getPos(), _UndeadTransform->getPos());
			Vector2D attackDirection(vector->direcionX, vector->direcionY);
			do
			{
				if (0 != _dungeonfloor->checkCollisions(_UndeadTransform->getPos().getX() + vector->direcionX + i, _UndeadTransform->getPos().getY() + vector->direcionY + i))
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
				else (i++);
			} while (chocan);

			if (elapsed >= attackspeed) {
				lastActionTime = now;

				// Ataque sin importar distancia
				_UndeadTransform->getVel() = Vector2D(0, 0); // detenerse

				auto* atk = _ent->getMngr()->getComponent<UndeadAttackComponent>(_ent);
				if (atk) {
					atk->GetVector(attackDirection);
					atk->Shoot(); 
				}

				isMoving = !isMoving;
				if (isMoving) {
					movement->ChooseRandom();
				}
			}
			else if (isMoving) {
				movement->Move();
			}
		}
	};
}


