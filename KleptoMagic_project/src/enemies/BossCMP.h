#include "../ecs/Component.h"
#include "../ecs/ecs.h"
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
#include "../ecs/ecs_defs_example.h"
#include "../enemies/enemyHoming.h"
#include "../bullet/BulletStats.h"
#include "../ecs/Transform.h"
#include "../sdlutils/Texture.h"
#include "../map/DungeonFloor.h"
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
			// Calculate the direction vector from the enemy to the player
			direcionX = playerPos.getX() - enemyPos.getX();
			direcionY = playerPos.getY() - enemyPos.getY();

			// Calculate the magnitude of the vector
			magnitude = std::sqrt(direcionX * direcionX + direcionY * direcionY);

			// Evitamos divisiones por 0
			if (magnitude > 0.0001) {
				direcionX /= magnitude;  // Normalize X
				direcionY /= magnitude;  // Normalize Y
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

		}

		
		void Teleport() 
		{
			int resultX = 1;
			float newX = _BossTransform->getPos().getX();
			float newY = _BossTransform->getPos().getY();
			float prevX = _BossTransform->getPos().getX();
			float prevY = _BossTransform->getPos().getY();

			while (resultX != 0) {

				if (_BossTransform) {
					// Generate a random position within a certain range of the current position
					newX += (std::rand() % 200 - 100);
					newY += (std::rand() % 200 - 100);

					
					// Make sure the new position is not too close to the player
					while (std::sqrt((newX - _player->getPos().getX()) * (newX - _player->getPos().getX()) +
						(newY - _player->getPos().getY()) * (newY - _player->getPos().getY())) < 100) {
						newX += (std::rand() % 200 - 100);
						newY += (std::rand() % 200 - 100);
					}
					/*
					
					while (std::sqrt((newX - _BossTransform->getPos().getX()) * (newX - _BossTransform->getPos().getX()) +
						(newY - _BossTransform->getPos().getY()) * (newY - _BossTransform->getPos().getY())) < 100) {
						newX = _BossTransform->getPos().getX() + (std::rand() % 200 - 100);
						newY = _BossTransform->getPos().getY() + (std::rand() % 200 - 100);
					}
					*/

					// Verificar si la nueva posición está dentro de los límites de la sala
					resultX = floor->checkCollisions(newX, newY);

					if (resultX != 0)
					{
						// Si la nueva posición está fuera de los límites, volver a la posición anterior
						newX = prevX;
						newY = prevY;
					}

					// if resultX is 0, the new position is valid
				}
			}
			// Update the boss's position
			_BossTransform->getPos().set(newX, newY);
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
		std::chrono::steady_clock::time_point lastTeleportTime = std::chrono::steady_clock::now();
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
			teleportCooldown = std::chrono::duration<float>(now - lastTeleportTime).count();

			float distance = vector->CreateVector(_player->getPos(), _BossTransform->getPos());
			Vector2D attackdirection(vector->direcionX * 1, vector->direcionY * 1);
			attackRange = distance;

			if (elapsedTime >= 4 && attackRange <= 300)
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
					Attack2();
					break;
				}

				lastAttackTime = now;
				_BossTransform->getVel() = _BossTransform->getVel() * 0;
			}
			if (teleportCooldown >= 4 && attackRange < 80)
			{
				movement->Teleport();
				lastTeleportTime = now;
			}

		}

		// Different attack patterns 

		// Attack 1, shoots 2 homing bullets
		void Attack1()
		{
			auto bullet = _ent->getMngr()->addEntity(ecs::grp::ENEMYBULLET);
			auto s = 50.0f;
			auto tr = _ent->getMngr()->addComponent<Transform>(bullet);
			// Set the position of the bullet to the boss's position with a small offset
			Vector2D bulletPos = _BossTransform->getPos();
			bulletPos.setX(bulletPos.getX() + 20); // Adjust the offset as needed
			bulletPos.setY(bulletPos.getY() - 20); // Adjust the offset as needed

			tr->init(bulletPos, Vector2D(), s, s, 0.0f);
			_ent->getMngr()->addComponent<Image>(bullet, &sdlutils().images().at("enemy_bullet"));
			_ent->getMngr()->addComponent<enemyHoming>(bullet);
			_ent->getMngr()->addComponent<BulletStats>(bullet);
			auto stats = _ent->getMngr()->getComponent<BulletStats>(bullet);
			stats->enemyStats(3);

			auto bullet2 = _ent->getMngr()->addEntity(ecs::grp::ENEMYBULLET);
			auto tr2 = _ent->getMngr()->addComponent<Transform>(bullet2);
			// Set the position of the bullet to the boss's position with a small offset
			Vector2D bulletPos2 = _BossTransform->getPos();
			bulletPos2.setX(bulletPos2.getX() - 20); // Adjust the offset as needed
			bulletPos2.setY(bulletPos2.getY() - 20); // Adjust the offset as needed

			tr2->init(bulletPos2, Vector2D(), s, s, 0.0f);
			_ent->getMngr()->addComponent<Image>(bullet2, &sdlutils().images().at("enemy_bullet"));
			_ent->getMngr()->addComponent<enemyHoming>(bullet2);
			_ent->getMngr()->addComponent<BulletStats>(bullet2);
			auto stats2 = _ent->getMngr()->getComponent<BulletStats>(bullet2);	
			stats2->enemyStats(4);
			if (!stats->getPiercing())
			{
				RoomStorage* roomstorage = new RoomStorage();
				DungeonFloor* dungeonfloor = new DungeonFloor(10, 10, 10, 10, 10, roomstorage, sdlutils().renderer());
				auto* tilechecker = _ent->getMngr()->addComponent<TileCollisionChecker>(bullet);
				tilechecker->init(true, tr,dungeonfloor );
				tr->initTileChecker(tilechecker);
			}if (!stats->getPiercing())
			{
				RoomStorage* roomstorage = new RoomStorage();
				DungeonFloor* dungeonfloor = new DungeonFloor(10, 10, 10, 10, 10, roomstorage, sdlutils().renderer());
				auto* tilechecker = _ent->getMngr()->addComponent<TileCollisionChecker>(bullet2);
				tilechecker->init(true, tr2,dungeonfloor );
				tr2->initTileChecker(tilechecker);
			}

		}

		// Attack 2, shoots multiple bullets in a spread pattern
		// still incomplete
		void Attack2()
		{
			auto bullet = _ent->getMngr()->addEntity(ecs::grp::ENEMYBULLET);
			auto s = 50.0f;
			auto tr = _ent->getMngr()->addComponent<Transform>(bullet);
			// Set the position of the bullet to the boss's position with a small offset
			Vector2D bulletPos = _BossTransform->getPos();
			bulletPos.setX(bulletPos.getX() + 20); // Adjust the offset as needed
			bulletPos.setY(bulletPos.getY() - 20); // Adjust the offset as needed
			tr->init(bulletPos, Vector2D(), s, s, 0.0f);
			_ent->getMngr()->addComponent<Image>(bullet, &sdlutils().images().at("enemy_bullet"));
			_ent->getMngr()->addComponent<BulletStats>(bullet);
			auto stats = _ent->getMngr()->getComponent<BulletStats>(bullet);
			stats->enemyStats(2);
			if (!stats->getPiercing())
			{
				RoomStorage* roomstorage = new RoomStorage();
				DungeonFloor* dungeonfloor = new DungeonFloor(10, 10, 10, 10, 10, roomstorage, sdlutils().renderer());
				auto* tilechecker = _ent->getMngr()->addComponent<TileCollisionChecker>(bullet);
				tilechecker->init(true, tr, dungeonfloor);
			}
		}

	};
}
