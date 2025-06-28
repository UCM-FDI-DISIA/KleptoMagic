#include "../ecs/Component.h"
#include "../ecs/ecs.h"
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
#include "../ecs/ecs_defs_example.h"
#include "../ecs/Transform.h"
#include "../sdlutils/Texture.h"
#include "../render/AnimatorComponent.h"
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
				Vector2D attackdirection(vector->direcionX , vector->direcionY);
				attackRange = vector->magnitude;

				if (elapsedTime >= attackspeed && attackRange <= range)
				{
					//create bullet
					
					lastAttackTime = now;
					_UndeadTransform->getVel() =  _UndeadTransform->getVel() * 0;
				}
				if (attackRange > range) 
				{
					movement->Move();
				}			 
		}
	};

	class UndeadAnimComponent : public AnimatorComponent {
	public:
		__CMPID_DECL__(ecs::cmp::UNDEADANIMCMP);

		enum AnimationState {
			IDLE,
			WALKING,
			DYING
		};

		UndeadAnimComponent() {
			currentState = IDLE;
			isFacingRight = false;
		}

		void initComponent() override {
			AnimatorComponent::initComponent();
			// Configuración para 8 frames (2 filas x 4 columnas)
			startFrame = 0;   // Primer frame de idle (usaremos el primer frame de walking)
			deathFrame = 4;    // Primer frame de muerte (fila 2)
			walkFrame = 0;     // Primer frame de caminar (fila 1)
		}

		void update() override {
			// Solo procesar animaciones si no está muriendo
			if (currentState != DYING) {
				float velX = _tr->getVel().getX();
				float velY = _tr->getVel().getY();
				bool isCurrentlyMoving = (velX != 0 || velY != 0);
				bool isCurrentlyMovingRight = (velX < 0);

				// Transiciones de estado
				if (isCurrentlyMoving && currentState != WALKING) {
					currentState = WALKING;
					setAnimation(WALKING);
				}
				else if (!isCurrentlyMoving && currentState != IDLE) {
					currentState = IDLE;
					setAnimation(IDLE);
				}

				// Dirección de mirada
				if (isCurrentlyMoving && velX != 0) {
					if (!isFacingRight && !isCurrentlyMovingRight) {
						toggleFlip();
					}
					else if (isFacingRight && isCurrentlyMovingRight) {
						toggleFlip();
					}
				}
			}
		}

		void playDeath() override {
			currentState = DYING;
			AnimatorComponent::playDeath();
		}

	private:
		AnimationState currentState;
		int walkFrame;

		void setAnimation(AnimationState state) {
			switch (state) {
			case IDLE:
				// Usamos el primer frame de walking como idle
				_img->setStartingFrame(walkFrame);
				_img->setFrame(walkFrame);
				_img->setNumFrames(1); // Solo 1 frame para idle
				break;
			case WALKING:
				_img->setStartingFrame(walkFrame);
				_img->setFrame(walkFrame);
				_img->setNumFrames(4); // 4 frames para caminar
				break;
			case DYING:
				_img->setStartingFrame(deathFrame);
				_img->setFrame(deathFrame);
				_img->setNumFrames(4); // 4 frames para muerte
				break;
			}
		}
	};
}
