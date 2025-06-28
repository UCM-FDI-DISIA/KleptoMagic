#include "../ecs/Component.h"
#include "../ecs/ecs.h"
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
#include "../ecs/ecs_defs_example.h"
#include "../ecs/Transform.h"
#include "../sdlutils/Texture.h"
#include "../render/AnimatorComponent.h"
#include "../bullet/BulletUtils.h"
#include "../enemies/SlimeComponents.h"
#include <chrono>

class UndeadBulletCollisionComponent : public Component {
public:
	__CMPID_DECL__(ecs::cmp::UNDEADBULLETCOLLISION);

	Transform* bulletTransform;
	Transform* playerTransform;
	BulletStats* bulletStats;

	void initComponent() override {
		auto* mngr = _ent->getMngr();
		bulletTransform = mngr->getComponent<Transform>(_ent);
		playerTransform = mngr->getComponent<Transform>(mngr->getHandler(ecs::hdlr::PLAYER));
		bulletStats = mngr->getComponent<BulletStats>(_ent);
	}

    void update() override {
        if (!bulletTransform || !playerTransform || !bulletStats)
            return;

        float dist = std::hypot(
            bulletTransform->getPos().getX() - playerTransform->getPos().getX(),
            bulletTransform->getPos().getY() - playerTransform->getPos().getY()
        );

        float collisionDistance = 25.0f; // Ajusta según tamaño del jugador y bala

        if (dist < collisionDistance) {
            auto* mngr = _ent->getMngr();
            auto playerEntity = mngr->getHandler(ecs::hdlr::PLAYER);
            auto* playerStats = mngr->getComponent<EntityStat>(playerEntity);

            if (playerStats != nullptr) {
                // Aplicar daño al jugador
                playerStats->ChangeStat(-bulletStats->getArcherDamage(), EntityStat::Stat::HealthCurrent);

            }

            // Marcar la entidad para ser eliminada - FORMA CORRECTA
            mngr->setAlive(_ent, false);  // Usar el manager para cambiar el estado
        }
    }
};