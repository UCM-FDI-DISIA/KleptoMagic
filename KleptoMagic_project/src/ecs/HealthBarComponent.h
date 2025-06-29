#pragma once
#include "../ecs/Component.h"
#include "EntityStat.h"
#include "../render/Camera.h"
#include <SDL_pixels.h>

class Transform;

class HealthBarComponent : public ecs::Component {
public:
    __CMPID_DECL__(ecs::cmp::HEALTHBAR)

        HealthBarComponent(EntityStat* stats, float offsetY = -10.0f);
    virtual ~HealthBarComponent();

    void initComponent() override;
    void render() override;
    void update() override;

private:
    EntityStat* stats_;
    float offsetY_;
    int originalWidth_;
    int originalHeight_;
    Transform* enemyTransform_;

    SDL_Color backgroundColor_;
    SDL_Color healthColor_;
};
