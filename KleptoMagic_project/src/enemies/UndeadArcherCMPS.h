#include "../ecs/Component.h"
#include "../ecs/ecs.h"
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
#include "../ecs/ecs_defs_example.h"
#include "../ecs/Transform.h"
#include "../sdlutils/Texture.h"
#include "../render/AnimatorComponent.h"
#include "../bullet/BulletUtils.h"
#include "../map/DungeonFloor.h"
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

	class UndeadAttackComponent : public Component {
	public:
		Transform* _UndeadTransform = nullptr;
		Transform* _player = nullptr;
		std::chrono::steady_clock::time_point lastAttackTime = std::chrono::steady_clock::now();
		float attackRange = 0.0f;
		float attackspeed = 0.0f;
		float range = 0.0f;
		BulletUtils* bulletUtils = nullptr;
		DungeonFloor* floor = nullptr;
		UndeadStatComponent* stat = nullptr;

		__CMPID_DECL__(ecs::cmp::UNDEADATKCMP);

		void initComponent() override {
			Manager* mngr = _ent->getMngr();
			_UndeadTransform = mngr->getComponent<Transform>(_ent);
			_player = mngr->getComponent<Transform>(mngr->getHandler(ecs::hdlr::PLAYER));
			stat = mngr->getComponent<UndeadStatComponent>(_ent);

			if (stat) {
				range = stat->attackrange;
				attackspeed = stat->attackspeed;
			}
		}

		~UndeadAttackComponent() {
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
			UndeadStatComponent* currentStat = mngr->getComponent<UndeadStatComponent>(_ent);
			UndeadMovementComponent* movement = mngr->getComponent<UndeadMovementComponent>(_ent);

			if (!vector || !currentStat || !movement || !_UndeadTransform || !_player) return;

			auto now = std::chrono::steady_clock::now();
			float elapsedTime = std::chrono::duration<float>(now - lastAttackTime).count();

			vector->CreateVector(_player->getPos(), _UndeadTransform->getPos());
			attackRange = vector->magnitude;

			movement->Move();

			if (elapsedTime >= attackspeed && attackRange <= range) {
				Vector2D direction = _player->getPos() - _UndeadTransform->getPos();
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

				Vector2D spawnPos = _UndeadTransform->getPos() +
					Vector2D(_UndeadTransform->getWidth() / 2, _UndeadTransform->getHeight() / 2) -
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

				lastAttackTime = now;
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
