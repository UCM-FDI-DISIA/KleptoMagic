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
		__CMPID_DECL__(ecs::cmp::ARMORVECCMP);
		float direcionX, direcionY;
		Vector2D _dest;

		void initComponent() override
		{
			auto* _mngr = _ent->getMngr();
			CreateVector();
		}
		void CreateVector() {
			// Generar direcci�n aleatoria dentro de la sala
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_real_distribution<float> dist(0.0f, 400.0f);
			Vector2D currentDir_ = Vector2D(dist(gen), dist(gen));
			direcionX = currentDir_.getX();
			direcionY = currentDir_.getY();
			_dest = Vector2D(direcionX, direcionY);
		}
	};

	class ArmorStatComponent : public Component
	{
		Transform* _armorTransform;
		Transform* _playerTransform;
	public:
		__CMPID_DECL__(ecs::cmp::ARMORSTATCMP);
		float speed = 0.1;
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

		// Cada cierto tiempo ataca en la direcci�n en la que se mueve, generando un ataque 
		void update() override
		{
			auto* _mngr = _ent->getMngr();
			auto stat = static_cast<ArmorStatComponent*>(_ent->getMngr()->getComponent<ArmorStatComponent>(_ent));
			auto vector = static_cast<ArmorVectorComponent*>(_ent->getMngr()->getComponent<ArmorVectorComponent>(_ent));
			auto armorTransform = _mngr->getComponent<Transform>(_ent);
			// Verificar si ha pasado el tiempo de recarga
			auto now = std::chrono::steady_clock::now();
			auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastAttackTime).count();
			if (elapsedTime >= attackCooldown) {
				// Realizar ataque
				attackCooldown = 1000; // 1 segundo de cooldown
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
			float directionChangeTime_ = 1500.0f;		//Time to change direction in milliseconds

			/*
			timer_ += sdlutils().virtualTimer().currRealTime();
			if (timer_ >= directionChangeTime_) {
				// Generar direcci�n aleatoria
				vector->CreateVector();
				Vector2D velocity(vector->direcionX * stat->speed, vector->direcionY * stat->speed);
				_armorTransform->getVel() = velocity;
				timer_ = 0;
				sdlutils().virtualTimer().resetTime();
			}
			*/
			// Posici�n actual del asteroide
			Vector2D currentPos = _armorTransform->getPos();
			float distance = (vector->_dest - currentPos).magnitude();


			// 1. Verificar si est� cerca del destino
			if (distance < 60) {
				vector->CreateVector(); // Cambiar destino
			}

			// 2. Calcular direcci�n al destino (q - p)
			Vector2D dir = (vector->_dest - currentPos).normalize();

			_armorTransform->getVel() = dir;

			// Mover la entidad
			/*
			Vector2D newPos = _armorTransform->getPos() + (_armorTransform->getVel() * stat->speed * sdlutils().virtualTimer().currRealTime());
			_armorTransform->getPos().set(newPos);
			*/
		}
	};

	class ArmorAnimComponent : public AnimatorComponent {
		friend AnimatorComponent;
	public:
		__CMPID_DECL__(ecs::cmp::SLIMEANIMCMP);

		ArmorAnimComponent() {
			isWalking = false;
			isFacingRight = false;
			isFacingDown = true;
		}

		void update() override {
			// check speed to see if it's walking or not
			float velX = _tr->getVel().getX();
			float velY = _tr->getVel().getY();

			bool isCurrentlyMovingSideways = (velX != 0);
			bool isCurrentlyMovingVertically = (velY != 0);
			bool isCurrentlyMovingRight = (velX < 0);
			bool isCurrentlyMovingDown = (velY > 0);
			//cout << velX << "|" << velY << "   " 
			//	<< isCurrentlyMovingSideways << "|" << isCurrentlyMovingVertically << "   " 
			//	<< isCurrentlyMovingRight << "|" << isCurrentlyMovingDown << endl;


			if (!isFacingDown && !isCurrentlyMovingDown && isCurrentlyMovingVertically) {
				// up
				isFacingDown = !isFacingDown;
				changeDirection('U');
			}
			else if (isFacingDown && isCurrentlyMovingDown && isCurrentlyMovingVertically) {
				// down
				isFacingDown = !isFacingDown;
				changeDirection('D');
			}
			else if (!isFacingRight && !isCurrentlyMovingRight && isCurrentlyMovingSideways) {
				// left
				isFacingRight = !isFacingRight;
				changeDirection('L');
			}
			else if (isFacingRight && isCurrentlyMovingRight && isCurrentlyMovingSideways) {
				// right
				isFacingRight = !isFacingRight;
				changeDirection('R');
			}
		}

		void changeDirection(char dir) {
			switch (dir) {
			case 'U':
				offset = 1;
				break;
			case 'D':
				offset = 0;
				break;
			case 'L':
				offset = 3;
				break;
			case 'R':
				offset = 2;
				break;
			}
			_img->setStartingFrame(startFrame + offset);
			_img->setFrame(startFrame + 1);
			_img->setNumFrames(4);
		}

		char facingDirection() {
			if (offset == 0) return 'D';
			else if (offset == 1) return 'U';
			else if (offset == 2) return 'L';
			else if (offset == 3) return 'R';
		}

	private:
		bool isFacingDown;
		int offset;
		void createStart();
	};
}

	// class ArmorCollisionComponent : public Component clase para colisiones, teniendo en cuenta el tama�o del armor y el player y que la armadura no recibe da�o si el ataque viene desde la direccion en la que se mueve
	/*
	class ArmorCollisionComponent : public Component
	{
		Transform* _armorTransform;
		Transform* _player;
	public:
		__CMPID_DECL__(ecs::cmp::ARMORCOLCMP);
		void initComponent() override
		{
			auto* _mngr = _ent->getMngr();
			_armorTransform = _mngr->getComponent<Transform>(_ent);
			_player = _mngr->getComponent<Transform>(_mngr->getHandler(ecs::hdlr::PLAYER));
		}
		void update() override
		{
			auto stat = static_cast<ArmorStatComponent*>(_ent->getMngr()->getComponent<ArmorStatComponent>(_ent));
			auto attack = static_cast<ArmorAttackComponent*>(_ent->getMngr()->getComponent<ArmorAttackComponent>(_ent));
			auto vector = static_cast<ArmorVectorComponent*>(_ent->getMngr()->getComponent<ArmorVectorComponent>(_ent));
			if (_armorTransform->collides(_player))
			{
				attack->Colision();
				stat->damage = 0;
				vector->direcionX = 0;
				vector->direcionY = 0;
			}
			else
			{
				stat->damage = 10;
				vector->direcionX = 1;
				vector->direcionY = 1;
			}
		}
	};
	*/