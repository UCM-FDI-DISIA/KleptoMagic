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
const float DEFAULT_COOLDOWN_TIMER = 1;

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
			std::uniform_int_distribution<int> rndDistributor(0.0f, 2 * 3.14);
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

	class GhostAttackComponent : public Component {
	private:

	public:
		__CMPID_DECL__(cmp::GHOSTATKCMP);

		void update() override{}


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

