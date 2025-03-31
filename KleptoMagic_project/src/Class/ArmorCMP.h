#include "../ecs/Component.h"
#include "../ecs/ecs.h"
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
#include "../ecs/ecs_defs.h"
#include "../sdlutils/SDLUtils.h"
#include "Transform.h"
#include <random>

#include <chrono>
namespace ecs
{
	class ArmorVectorComponent : public Component
	{
	private:

		Transform* _armorTransform;
		Transform* _playerTransform;
	public:
		__CMPID_DECL__(ecs::cmp::SLIMEVECCMP);
		float direcionX, direcionY;

		void initComponent() override
		{
			auto* _mngr = _ent->getMngr();
		}
		void CreateVector(Vector2D playerPos, Vector2D enemyPos) {
			direcionX = enemyPos.getX() - playerPos.getX();
			direcionY = enemyPos.getY() - playerPos.getY();
			float length = std::sqrt(direcionX * direcionX + direcionY * direcionY);
			if (length != 0) {
				direcionX /= length;
				direcionY /= length;
			}
		}
	};


	class ArmorStatComponent : public Component
	{

	public:
		__CMPID_DECL__(ecs::cmp::SLIMESTATCMP);
		float speed = 0.5;
		float damage = 10;
		float attackspeed = 1;

		void initComponent() override
		{
			auto* _mngr = _ent->getMngr();
		}
		void update() override {}
	};



	class ArmorMovementComponent : public Component
	{

		Transform* _armorTransform;
		Transform* _player;
		Vector2D currentDir_;				// Dirección actual de movimiento
		float movementSpeed_;				// Velocidad de movimiento
		float directionChangeTime_ = 5.0f;	// Cada cuánto cambia dirección (segundos)
		float timer_ = 0.0f;				// Temporizador interno
	public:
		__CMPID_DECL__(ecs::cmp::SLIMEMOVCMP);

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

			if (vector && stat && _armorTransform)
			{
				/*
				vector->CreateVector(_slimeTransform->getPos(), _player->getPos());
				Vector2D velocity(vector->direcionX * 0.5, vector->direcionY * 0.5);
				_slimeTransform->getVel() = velocity;
				*/
				// Actualizar temporizador y cambiar dirección si es necesario
				timer_ += sdlutils().virtualTimer().currRealTime();
				if (timer_ >= directionChangeTime_) {
					// Generar dirección aleatoria
					std::random_device rd;
					std::mt19937 gen(rd());
					std::uniform_real_distribution<float> dist(-1.0f, 1.0f);
					currentDir_ = Vector2D(dist(gen), dist(gen)).normalize();
					timer_ = 0;
				}

				// Mover la entidad
				Vector2D newPos = _armorTransform->getPos() + (currentDir_ * movementSpeed_ * sdlutils().virtualTimer().currRealTime());
				_armorTransform->getPos().set(newPos);
			}


		}
	};

	class ArmorAttackComponent : public Component
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


			auto stat = static_cast<ArmorStatComponent*>(_ent->getMngr()->getComponent<ArmorStatComponent>(_ent));
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