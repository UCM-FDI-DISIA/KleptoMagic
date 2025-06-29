#include "HealthBarComponent.h"
#include "../ecs/Transform.h"
#include "../render/Camera.h"
#include "../sdlutils/SDLUtils.h"

HealthBarComponent::HealthBarComponent(EntityStat* stats, float offsetY)
    : stats_(stats),
    offsetY_(offsetY),
    enemyTransform_(nullptr),
    originalWidth_(50),
    originalHeight_(8),
    backgroundColor_{ 255, 255, 255, 255 },  // blanco
    healthColor_{ 255, 0, 0, 255 }           // rojo
{
}

HealthBarComponent::~HealthBarComponent() {}

void HealthBarComponent::initComponent() {
    enemyTransform_ = _ent->getMngr()->getComponent<Transform>(_ent);
}

void HealthBarComponent::update() {
    // No se necesita lógica en update por ahora.
}

void HealthBarComponent::render() {
    if (stats_ == nullptr || enemyTransform_ == nullptr) return;

    SDL_Renderer* renderer = sdlutils().renderer();

    // Posición del enemigo teniendo en cuenta la cámara
    Vector2D enemyPos = enemyTransform_->getPos() - camOffset;

    int baseX = static_cast<int>(enemyPos.getX() +
        (enemyTransform_->getWidth() - originalWidth_) / 2);
    int baseY = static_cast<int>(enemyPos.getY() + offsetY_);

    // Fondo (barra vacía)
    SDL_Rect bgRect = { baseX, baseY, originalWidth_, originalHeight_ };
    SDL_SetRenderDrawColor(renderer, backgroundColor_.r, backgroundColor_.g, backgroundColor_.b, backgroundColor_.a);
    SDL_RenderFillRect(renderer, &bgRect);

    // Barra de vida (relleno rojo)
    float healthPercentage = stats_->getStat(EntityStat::Stat::HealthCurrent) /
        stats_->getStat(EntityStat::Stat::HealthTotal);
    healthPercentage = std::max(0.0f, std::min(1.0f, healthPercentage));

    int healthWidth = static_cast<int>(originalWidth_ * healthPercentage);
    if (healthWidth > 0) {
        SDL_Rect healthRect = { baseX, baseY, healthWidth, originalHeight_ };
        SDL_SetRenderDrawColor(renderer, healthColor_.r, healthColor_.g, healthColor_.b, healthColor_.a);
        SDL_RenderFillRect(renderer, &healthRect);
    }
}
