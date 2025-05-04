#include "../ecs/Component.h"
#include "../ecs/ecs.h"
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
#include "../ecs/ecs_defs_example.h"
#include "../Class/enemyHoming.h"
#include "../Class/BulletStats.h"
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
		float CreateVector(Vector2D playerPos, Vector2D enemyPos) {
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

			return magnitude;
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



		DungeonFloor* floor;
		float speed;

	public:
		__CMPID_DECL__(ecs::cmp::BOSSMOVCMP);

		void init(DungeonFloor* dFloor) {
			floor = dFloor;
		}

		void initComponent() override
		{
			auto* _mngr = _ent->getMngr();
			_BossTransform = _mngr->getComponent<Transform>(_ent);
			_player = _mngr->getComponent<Transform>(_mngr->getHandler(ecs::hdlr::PLAYER));

			//auto sala = _mngr->getHandler(ecs::hdlr::GAMEINFO);

			auto stat = static_cast<BossStatComponent*>(_ent->getMngr()->getComponent<BossStatComponent>(_ent));
			speed = stat->speed;
		}

		
		void Teleport() 
		{
			if (_BossTransform) {
				// Generar una nueva posici�n alejada de la actual, teniendo en cuenta que est� dentro de la sala
				float newX = _BossTransform->getPos().getX() + (std::rand() % 200 - 100);
				float newY = _BossTransform->getPos().getY() + (std::rand() % 200 - 100);
				// Asegurarse de que la nueva posici�n est� dentro de los l�mites de la sala
				// 
				//
				// Aqu� puedes definir los l�mites de la sala, por ejemplo:
				float roomWidth = 800; // Ancho de la sala
				float roomHeight = 600; // Alto de la sala
				if (newX < 0) newX = 0;
				if (newX > roomWidth) newX = roomWidth;
				if (newY < 0) newY = 0;
				if (newY > roomHeight) newY = roomHeight;

				// Asegurarse de que la nueva posici�n est� suficientemente alejada del jugador
				while (std::sqrt((newX - _player->getPos().getX()) * (newX - _player->getPos().getX()) +
					(newY - _player->getPos().getY()) * (newY - _player->getPos().getY())) < 100) {
					newX = _BossTransform->getPos().getX() + (std::rand() % 200 - 100);
					newY = _BossTransform->getPos().getY() + (std::rand() % 200 - 100);
				}

				

				// Asegurarse de que la nueva posici�n est� suficientemente alejada
				while (std::sqrt((newX - _BossTransform->getPos().getX()) * (newX - _BossTransform->getPos().getX()) +
					(newY - _BossTransform->getPos().getY()) * (newY - _BossTransform->getPos().getY())) < 100) {
					newX = _BossTransform->getPos().getX() + (std::rand() % 200 - 100);
					newY = _BossTransform->getPos().getY() + (std::rand() % 200 - 100);
				}

				// Asignar la nueva posici�n al Transform del enemigo
				_BossTransform->getPos().set(newX, newY);
			if (vector && stat && _BossTransform)
			{
				float dist = std::hypot(_BossTransform->getPos().getX() - _player->getPos().getX(),
					_BossTransform->getPos().getY() - _player->getPos().getY());
				if (dist > 50)
				{
					auto path = floor->findPathToX(_BossTransform->getPos().getX() / 50, _BossTransform->getPos().getY() / 50, _player->getPos().getX() / 50, _player->getPos().getY() / 50);
					//std::cout << Vector2D(path[1].x * 50, path[1].y * 50) << endl;

					if (path.size() > 0)
					{
						vector->CreateVector(Vector2D(path[1].x * 50, path[1].y * 50), _BossTransform->getPos());
						Vector2D velocity(vector->direcionX * speed, vector->direcionY * speed);
						_BossTransform->getVel() = velocity;
					}
				}
				else
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
		float teleportCooldown = 5.0f;
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
			auto now = std::chrono::steady_clock::now();
			float elapsedTime = std::chrono::duration<float>(now - lastAttackTime).count();

			float distance = vector->CreateVector(_player->getPos(), _BossTransform->getPos());
			Vector2D attackdirection(vector->direcionX * 1, vector->direcionY * 1);
			attackRange = distance;

			if (elapsedTime >= 5 && attackRange <= 300)
			{
				//chooses one attack from all possible attack patterns

				//int attackPattern = rand() % 2; // Randomly choose an attack pattern (0 or 1)
				int attackPattern = 0; // For testing purposes, always use attack pattern 0
				switch (attackPattern)
				{
				case 0:
					Attack1();
					break;
				case 1:
					// Attack2();
					break;
				}

				lastAttackTime = now;
				_BossTransform->getVel() = _BossTransform->getVel() * 0;
			}
			if (attackRange < 75)
			{
				movement->Teleport();
			}

		}

		// Different attack patterns 

		// Attack 1, shoots 2 homing bullets
		void Attack1()
		{
			auto bullet = _ent->getMngr()->addEntity(ecs::grp::ENEMY);
			auto s = 50.0f;
			auto tr = _ent->getMngr()->addComponent<Transform>(bullet);
			tr->init(_BossTransform->getPos(), Vector2D(), s, s, 0.0f);
			_ent->getMngr()->addComponent<Image>(bullet, &sdlutils().images().at("tennis_ball"));
			_ent->getMngr()->addComponent<enemyHoming>(bullet);
			_ent->getMngr()->addComponent<BulletStats>(bullet);
			auto stats = _ent->getMngr()->getComponent<BulletStats>(bullet);
			stats->enemyStats(1);
			//_ent->getMngr()->addComponent<BulletMovement>(bullet);
		}

	};
}
