#include "LivingArmorComponent.h"
#include "../sdlutils/SDLUtils.h"
#include <random>



LivingArmorComponent::LivingArmorComponent(float speed = 100.0f, float dirChangeTime = 2.0f) {
	movementSpeed_ = speed;
	directionChangeTime_ = dirChangeTime;
	timer_ = 0.0f;
}

LivingArmorComponent::~LivingArmorComponent() {
}

void LivingArmorComponent::initComponent() {
	// Obtener el Transform de la entidad (aseg�rate de que existe)
	auto mngr = _ent->getMngr();
	_armorTransform = mngr->getComponent<Transform>(_ent);

	// Obtener el Transform del jugador (aseg�rate de que existe)
	_playerTransform = mngr->getComponent<Transform>(mngr->getHandler(ecs::hdlr::FIGHTER));

}

void LivingArmorComponent::update() {
    // Actualizar temporizador y cambiar direcci�n si es necesario
    timer_ += sdlutils().virtualTimer().currRealTime();
    if (timer_ >= directionChangeTime_) {
        // Generar direcci�n aleatoria (ejemplo en 2D)
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dist(-1.0f, 1.0f);
        currentDir_ = Vector2D(dist(gen), dist(gen)).normalize();
        timer_ = 0;
    }

    // Mover la entidad
    Vector2D newPos = _armorTransform->getPos() + (currentDir_ * movementSpeed_ * Time::getDeltaTime());
    _armorTransform->getPos().set(newPos);
}

bool LivingArmorComponent::isVulnerable(const Vector2D& attackerPosition) const {
    // Calcular vector hacia el atacante
    Vector2D toAttacker = (attackerPosition - _armorTransform->getPos()).normalize();

    // Obtener la direcci�n actual de la armadura (hacia donde mira)
    Vector2D forward = _armorTransform->getForward().normalize();

    // Calcular �ngulo entre los vectores
    float dot = forward.getX() * toAttacker.getX() + forward.getY() * toAttacker.getY();
    float angle = acos(dot) * 180.0 / M_PIl;

    // Vulnerable si el ataque viene de atr�s o lados (>90 grados)
    return (angle > 90.0f);
}

