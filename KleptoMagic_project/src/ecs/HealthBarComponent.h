#pragma once
#include "../ecs/Component.h"
#include "../sdlutils/Texture.h"
#include "EntityStat.h"
#include "../render/Camera.h"

class Transform; 

class HealthBarComponent : public ecs::Component {
public:
    __CMPID_DECL__(ecs::cmp::HEALTHBAR)

        HealthBarComponent(Texture* fullTex, Texture* emptyTex, EntityStat* stats, float offsetY = -10.0f);
    virtual ~HealthBarComponent();

    void initComponent() override;
    void render() override;
    void update() override;

private:
    Texture* fullHealthTexture_;
    Texture* emptyHealthTexture_;
    EntityStat* stats_;
    float offsetY_;
    int originalWidth_;
    int originalHeight_;
    Transform* enemyTransform_;
};