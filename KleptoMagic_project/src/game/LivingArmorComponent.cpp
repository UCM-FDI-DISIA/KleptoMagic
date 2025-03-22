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
        // Generar direcci�n aleatoria
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dist(-1.0f, 1.0f);
        currentDir_ = Vector2D(dist(gen), dist(gen)).normalize();
        timer_ = 0;
    }

    // Mover la entidad
    Vector2D newPos = _armorTransform->getPos() + (currentDir_ * movementSpeed_ * sdlutils().virtualTimer().currRealTime());
    _armorTransform->getPos().set(newPos);
}

// M�todo para comprobar si la armadura es vulnerable a un ataque, osea si el ataque viene de atr�s o de los lados, 
// basado en el �ngulo entre la direcci�n actual de la armadura y la direcci�n del atacante
bool LivingArmorComponent::isVulnerable(const Vector2D& attackerPosition) const {
    // Calcular vector hacia el atacante
    Vector2D toAttacker = (attackerPosition - _armorTransform->getPos()).normalize();

    // Obtener la direcci�n actual de la armadura (hacia donde mira)
    Vector2D forward = _armorTransform->getVel().normalize();

    // Calcular �ngulo entre los vectores
    float dot = forward.getX() * toAttacker.getX() + forward.getY() * toAttacker.getY();
    float angle = acos(dot) * 180.0 / M_PIl;

    // Vulnerable si el ataque viene de atr�s o lados (>90 grados)
    return (angle > 90.0f);
}

