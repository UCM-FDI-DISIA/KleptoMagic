#include "../ecs/Component.h"
#include "../ecs/ecs.h"
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
#include "../ecs/ecs_defs_example.h"
#include "../ecs/Transform.h"
#include "../sdlutils/Texture.h"
#include "../render/AnimatorComponent.h"
#include "../bullet/BulletUtils.h"
#include <chrono>

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
			attackspeed = 4.0f; 
			damage = 1;
			attackrange = 280; 
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
			// Calculamos el vector dirección
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
		DungeonFloor* floor;

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
		}

		void Move()

		{
			auto vector = static_cast<UndeadVectorComponent*>(_ent->getMngr()->getComponent<UndeadVectorComponent>(_ent));
			

			if (vector && stat && _UndeadTransform)
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
		}
	};

	class UndeadAttackComponent : public Component
	{
	public:
		Transform* _UndeadTransform;
		Transform* _player;
		std::chrono::steady_clock::time_point lastAttackTime = std::chrono::steady_clock::now();
		float attackRange;
		float attackspeed;
		float range;

		__CMPID_DECL__(ecs::cmp::UNDEADATKCMP);

		void initComponent() override
		{
			auto* _mngr = _ent->getMngr();
			_UndeadTransform = _mngr->getComponent<Transform>(_ent);
			_player = _mngr->getComponent<Transform>(_mngr->getHandler(ecs::hdlr::PLAYER));
			auto stat = static_cast<UndeadStatComponent*>(_ent->getMngr()->getComponent<UndeadStatComponent>(_ent));
			range = stat->attackrange;
			attackspeed = stat->attackspeed;
		}

		void update() override
		{
			auto vector = static_cast<UndeadVectorComponent*>(_ent->getMngr()->getComponent<UndeadVectorComponent>(_ent));
			auto movement = static_cast<UndeadMovementComponent*>(_ent->getMngr()->getComponent<UndeadMovementComponent>(_ent));

			auto now = std::chrono::steady_clock::now();
			float elapsedTime = std::chrono::duration<float>(now - lastAttackTime).count();

			vector->CreateVector(_player->getPos(), _UndeadTransform->getPos());
			attackRange = vector->magnitude;

			// Siempre intentar moverse
			movement->Move();

			// Disparar si está en rango y ha pasado el tiempo de recarga
			if (elapsedTime >= attackspeed && attackRange <= range)
			{
				BulletUtils bulletUtils;
				bulletUtils.undeadArcherShoot(_UndeadTransform, 0);
				lastAttackTime = now;

				// Pequeña reducción de velocidad durante el disparo (opcional)
				_UndeadTransform->getVel() = _UndeadTransform->getVel() * 0.7f;
			}
		}
	};


	class UndeadAnimComponent : public AnimatorComponent {
	public:
		__CMPID_DECL__(ecs::cmp::UNDEADANIMCMP);

		UndeadAnimComponent() {
			isWalking = false;
			isFacingRight = true; // Asumimos que el sprite original mira a derecha
			currentFlipState = false; // No flip inicialmente
		}

		void update() override {
			float velX = _tr->getVel().getX();
			bool isCurrentlyWalking = (velX != 0 || _tr->getVel().getY() != 0);
			bool isCurrentlyMovingRight = (velX > 0);

			// Actualizar estado de caminar/idle
			if (!isWalking && isCurrentlyWalking) {
				toggleWalkingAnim();
			}
			else if (isWalking && !isCurrentlyWalking) {
				toggleWalkingAnim();
			}

			// Lógica de flip mejorada
			if (velX != 0) {
				bool shouldFaceRight = velX > 0;

				// Solo cambiar flip si la dirección cambia
				if (shouldFaceRight != isFacingRight) {
					isFacingRight = shouldFaceRight;
					_img->setFlip(!isFacingRight); // Flip solo cuando mira a izquierda
					currentFlipState = !isFacingRight;
				}
			}
		}

		void toggleWalkingAnim() {
			if (!isWalking) {
				// Animación de caminar (4 frames)
				_img->setStartingFrame(0);
				_img->setFrame(0);
				_img->setNumFrames(4);
			}
			else {
				// Animación idle (primer frame)
				_img->setStartingFrame(0);
				_img->setFrame(0);
				_img->setNumFrames(1);
			}
			isWalking = !isWalking;
		}

		void playDeath() override {
			// Animación de muerte (frames 4-7)
			_img->setStartingFrame(4);
			_img->setFrame(4);
			_img->setNumFrames(4);
			// Mantener el flip actual
			_img->setFlip(currentFlipState);
		}

	private:
		bool isWalking;
		bool isFacingRight;
		bool currentFlipState; // Para trackear el estado actual del flip
	};
}
