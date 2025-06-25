#include "HealthBarComponent.h"
#include "../ecs/Transform.h"
#include "../render/Camera.h" 

HealthBarComponent::HealthBarComponent(Texture* fullTex, Texture* emptyTex, EntityStat* stats, float offsetY) :
    fullHealthTexture_(fullTex),
    emptyHealthTexture_(emptyTex),
    stats_(stats),
    offsetY_(offsetY),
    enemyTransform_(nullptr)
{
    originalWidth_ = fullHealthTexture_->getWidth() / 8;
    originalHeight_ = fullHealthTexture_->getHeight() / 8;
}

HealthBarComponent::~HealthBarComponent() {

}

void HealthBarComponent::initComponent() {
    enemyTransform_ = _ent->getMngr()->getComponent<Transform>(_ent);
}

void HealthBarComponent::update() {

}

void HealthBarComponent::render() {
    if (stats_ == nullptr || enemyTransform_ == nullptr) return;

    // Posición absoluta del enemigo (sin camOffset)
    Vector2D enemyPos = enemyTransform_->getPos();

    // Calculamos posición de la barra relativa al enemigo
    SDL_Rect dest;
    dest.x = static_cast<int>(enemyPos.getX() +
        (enemyTransform_->getWidth() - originalWidth_) / 2);
    dest.y = static_cast<int>(enemyPos.getY() + offsetY_);
    dest.w = originalWidth_;
    dest.h = originalHeight_ / 2;

    // Renderizamos barra vacía (fondo)
    emptyHealthTexture_->render(dest);

    // Calculamos ancho proporcional a la vida
    float healthPercentage = stats_->getStat(EntityStat::Stat::HealthCurrent) /
        stats_->getStat(EntityStat::Stat::HealthTotal);
    healthPercentage = std::max(0.0f, std::min(1.0f, healthPercentage));

    dest.w = static_cast<int>(originalWidth_ * healthPercentage);

    // Renderizamos barra de vida actual
    fullHealthTexture_->render(dest);
}