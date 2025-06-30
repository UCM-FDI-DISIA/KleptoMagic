#define _USE_MATH_DEFINES

#ifndef GHOST_COMPONENT_C
#define GHOST_COMPONENT_C

#include "..\ecs\Manager.h"
#include "..\ecs\Transform.h"
#include <chrono>
#include <random>
#include <iostream>
#include <cstdlib>
#include <math.h>

const float DEFAULT_TELE_RADIUS = 100;
const float DEFAULT_COOLDOWN_TIMER = 2;

namespace ecs {
#pragma once
	class GhostTeleportComponent : public Component {
	private:
		Transform* _ghostTransform;
		Transform* _targetTransform;
		std::default_random_engine rndGenerator;
		std::uniform_int_distribution<int> rndDistributor;

		int _teleRadius = DEFAULT_TELE_RADIUS;

	public:
		__CMPID_DECL__(cmp::GHOSTTPCMP);

		GhostTeleportComponent(int rad = DEFAULT_TELE_RADIUS) 
			: _teleRadius(rad) {
			std::uniform_int_distribution<int> rndDistributor(0.0f, 2 * M_PI);
		}

		void initComponent() { // Gets it's transform and target's, and the timer
			auto* _mngr = _ent->getMngr();
			_ghostTransform = _mngr->getComponent<Transform>(_ent);
			_targetTransform = _mngr->getComponent<Transform>(_mngr->getHandler(ecs::hdlr::PLAYER));

			assert(_ghostTransform != nullptr);
			assert(_targetTransform != nullptr);
		}

		void teleportRndPosition() {
			Vector2D result;
			float rndAng = rndDistributor(rndGenerator);
			result.setX(_targetTransform->getPos().getX() + (_teleRadius * cos(rndAng)));
			result.setY(_targetTransform->getPos().getY() + (_teleRadius * sin(rndAng)));

			_ghostTransform->getPos() = result;
		}

		void update() override {}
	};

	class GhostStatComponent : public StatComponent
	{
	public:
		__CMPID_DECL__(ecs::cmp::GHOSTSTATCMP);

		void initComponent() override
		{
			auto* _mngr = _ent->getMngr();
			life = 8;
			speed = 1;
			attackspeed = 3.0f;
			damage = 1;
			attackrange = 280;
		}
	};


	class GhostAttackComponent : public Component {
	public:
		Transform* _GhostTransform = nullptr;
		Transform* _player = nullptr;
		std::chrono::steady_clock::time_point lastAttackTime = std::chrono::steady_clock::now();
		float attackRange = 0.0f;
		float attackspeed = 4.0f;
		float range = 0.0f;
		BulletUtils* bulletUtils = nullptr;
		DungeonFloor* floor = nullptr;
		UndeadStatComponent* stat = nullptr;

		__CMPID_DECL__(ecs::cmp::GHOSTATKCMP);

		void initComponent() override {
			Manager* mngr = _ent->getMngr();
			_GhostTransform = mngr->getComponent<Transform>(_ent);
			_player = mngr->getComponent<Transform>(mngr->getHandler(ecs::hdlr::PLAYER));
			stat = mngr->getComponent<UndeadStatComponent>(_ent);

			if (stat) {
				range = stat->attackrange;
				attackspeed = stat->attackspeed;
			}
		}

		~GhostAttackComponent() {
			if (bulletUtils) {
				delete bulletUtils;
				bulletUtils = nullptr;
			}
		}

		void init(DungeonFloor* f) {
			floor = f;
			if (!bulletUtils) {
				bulletUtils = new BulletUtils();
				bulletUtils->setDungeonFloor(floor);
			}
		}

		void update() override {
			Manager* mngr = _ent->getMngr();

			UndeadVectorComponent* vector = mngr->getComponent<UndeadVectorComponent>(_ent);
			GhostStatComponent* currentStat = mngr->getComponent<GhostStatComponent>(_ent);
			
			std::cout << !_player;

			auto now = std::chrono::steady_clock::now();
			float elapsedTime = std::chrono::duration<float>(now - lastAttackTime).count();

			vector->CreateVector(_player->getPos(), _GhostTransform->getPos());
			attackRange = vector->magnitude;


			if (elapsedTime >= attackspeed) {
				Vector2D direction = _player->getPos() - _GhostTransform->getPos();
				if (direction.magnitude() < 1e-3f) return;

				direction = direction.normalize();

				// Crear bala
				Entity* bullet = mngr->addEntity(ecs::grp::ENEMYBULLET);
				if (!bullet) return;

				BulletStats* stats = mngr->addComponent<BulletStats>(bullet);
				if (!stats) {
					mngr->setAlive(bullet, false);
					return;
				}
				stats->enemyStats(5);

				Transform* bulletTR = mngr->addComponent<Transform>(bullet);
				if (!bulletTR) {
					mngr->setAlive(bullet, false);
					return;
				}

				Vector2D spawnPos = _GhostTransform->getPos() +
					Vector2D(_GhostTransform->getWidth() / 2, _GhostTransform->getHeight() / 2) -
					Vector2D(stats->getSize() / 2, stats->getSize() / 2);

				Vector2D velocity = direction * stats->getSpeed();
				float rotation = -velocity.angle(Vector2D(0, -1));

				bulletTR->init(spawnPos, velocity, stats->getSize(), stats->getSize(), rotation);
				mngr->addComponent<Image>(bullet, &sdlutils().images().at("enemy_bullet"));
				mngr->addComponent<DestroyOnBorder>(bullet);

				if (!stats->getPiercing() && floor) {
					TileCollisionChecker* tileChecker = mngr->addComponent<TileCollisionChecker>(bullet);
					if (tileChecker) {
						tileChecker->init(true, bulletTR, floor);
						bulletTR->initTileChecker(tileChecker);
					}
				}
				else
				{
					mngr->addComponent<PlayerHitted>(bullet);
				}

				lastAttackTime = now;
			}
		}
	};

	class GhostAIComponent : public Component {
	private:
		GhostTeleportComponent* _ghostTpCmp;
		GhostAttackComponent* _ghostAtkCmp;
		std::chrono::steady_clock::time_point lastTeleportTime;

		int _cooldownTime = DEFAULT_COOLDOWN_TIMER;


	public:
		__CMPID_DECL__(cmp::GHOSTAICMP);

		GhostAIComponent(int timer = DEFAULT_COOLDOWN_TIMER)
			: _cooldownTime(timer) {
			std::chrono::steady_clock::time_point lastAttackTime = std::chrono::steady_clock::now();
		}

		void initComponent() {
			auto* _mngr = _ent->getMngr();
			_ghostTpCmp = _mngr->getComponent<GhostTeleportComponent>(_ent);
			_ghostAtkCmp = _mngr->getComponent<GhostAttackComponent>(_ent);

			assert(_ghostTpCmp != nullptr);
			assert(_ghostAtkCmp != nullptr);
		}

		void update() override {
			auto now = std::chrono::steady_clock::now();
			float elapsedTime = std::chrono::duration<float>(now - lastTeleportTime).count();

			if (elapsedTime >= _cooldownTime)
			{
				lastTeleportTime = now;
				_ghostTpCmp->teleportRndPosition();
			}
		}
	};
}

#endif // GHOST_COMPONENT_C

