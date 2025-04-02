#include "../ecs/Component.h"
#include "../ecs/ecs.h"
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
#include "../ecs/ecs_defs.h"
#include "Transform.h"
#include "../sdlutils/SDLUtils.h"
#include <chrono>
#include <random>

namespace ecs
{
	class ArmorVectorComponent : public Component
	{
	private:
		void createStart();

		Transform* _armorTransform;
		Transform* _playerTransform;
	public:
		__CMPID_DECL__(ecs::cmp::ARMORMOVCMP);
		float direcionX, direcionY;

		void initComponent() override
		{
			auto* _mngr = _ent->getMngr();
		}
		void CreateVector() {
			// Generar direcci�n aleatoria
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_real_distribution<float> dist(-1.0f, 1.0f);
			Vector2D currentDir_ = Vector2D(dist(gen), dist(gen)).normalize();
			direcionX = currentDir_.getX();
			direcionY = currentDir_.getY();
		}
	};

	class ArmorStatComponent : public Component
	{
		Transform* _armorTransform;
		Transform* _playerTransform;
	public:
		__CMPID_DECL__(ecs::cmp::ARMORSTATCMP);
		float speed = 0.5;
		float damage = 10;
		float attackspeed = 1;
		void initComponent() override
		{
			auto* _mngr = _ent->getMngr();
		}
		void update() override {}
	};

	class ArmorAttackComponent : public Component
	{
		Transform* _armorTransform;
		Transform* _player;
		bool colision = false;
	public:
		__CMPID_DECL__(ecs::cmp::ARMORATKCMP);
		float attackCooldown;
		std::chrono::steady_clock::time_point lastAttackTime = std::chrono::steady_clock::now();
		void initComponent() override
		{
			auto* _mngr = _ent->getMngr();
			_armorTransform = _mngr->getComponent<Transform>(_ent);
			_player = _mngr->getComponent<Transform>(_mngr->getHandler(ecs::hdlr::PLAYER));
		}
		void Colision()
		{
			colision = true;
		}
		void update() override
		{
			attackCooldown = 10;
			auto now = std::chrono::steady_clock::now();
			float elapsedTime = std::chrono::duration<float>(now - lastAttackTime).count();
			if (elapsedTime >= attackCooldown && colision)
			{
				// Hacer da�o al jugador
				//_player->getMngr()->getComponent<Health>(_player)->takeDamage(damage);
				lastAttackTime = now;
				colision = false;
			}
		}
	};

	class ArmorMovementComponent : public Component
	{
		Transform* _armorTransform;
		Transform* _player;
	public:
		__CMPID_DECL__(ecs::cmp::ARMORMOVCMP);
		void initComponent() override
		{
			auto* _mngr = _ent->getMngr();
			_armorTransform = _mngr->getComponent<Transform>(_ent);
			_player = _mngr->getComponent<Transform>(_mngr->getHandler(ecs::hdlr::PLAYER));
		}
		void update() override
		{
			auto vector = static_cast<ArmorVectorComponent*>(_ent->getMngr()->getComponent<ArmorVectorComponent>(_ent));
			auto stat = static_cast<ArmorStatComponent*>(_ent->getMngr()->getComponent<ArmorStatComponent>(_ent));
			
			// Actualizar temporizador y cambiar direcci�n si es necesario
			float timer_ = 0.0f;
			float directionChangeTime_ = 500.0f;		//Time to change direction in milliseconds

			timer_ += sdlutils().virtualTimer().currRealTime();
			if (timer_ >= directionChangeTime_) {
				// Generar direcci�n aleatoria
				vector->CreateVector();
				Vector2D velocity(vector->direcionX * 0.5, vector->direcionY * 0.5);
				_armorTransform->getVel() = velocity;
				timer_ = 0;
			}

			// Mover la entidad
			/*
			Vector2D newPos = _armorTransform->getPos() + (_armorTransform->getVel() * stat->speed * sdlutils().virtualTimer().currRealTime());
			_armorTransform->getPos().set(newPos);
			*/
		}
	};


}