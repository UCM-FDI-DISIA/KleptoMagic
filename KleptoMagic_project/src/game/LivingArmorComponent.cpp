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
	// Obtener el Transform de la entidad (asegúrate de que existe)
	auto mngr = _ent->getMngr();
	_armorTransform = mngr->getComponent<Transform>(_ent);

	// Obtener el Transform del jugador (asegúrate de que existe)
	_playerTransform = mngr->getComponent<Transform>(mngr->getHandler(ecs::hdlr::FIGHTER));

}

void LivingArmorComponent::update() {
    // Actualizar temporizador y cambiar dirección si es necesario
    timer_ += sdlutils().virtualTimer().currRealTime();
    if (timer_ >= directionChangeTime_) {
        // Generar dirección aleatoria
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

// Método para comprobar si la armadura es vulnerable a un ataque, osea si el ataque viene de atrás o de los lados, 
// basado en el ángulo entre la dirección actual de la armadura y la dirección del atacante
bool LivingArmorComponent::isVulnerable(const Vector2D& attackerPosition) const {
    // Calcular vector hacia el atacante
    Vector2D toAttacker = (attackerPosition - _armorTransform->getPos()).normalize();

    // Obtener la dirección actual de la armadura (hacia donde mira)
    Vector2D forward = _armorTransform->getVel().normalize();

    // Calcular ángulo entre los vectores
    float dot = forward.getX() * toAttacker.getX() + forward.getY() * toAttacker.getY();
    float angle = acos(dot) * 180.0 / M_PIl;

    // Vulnerable si el ataque viene de atrás o lados (>90 grados)
    return (angle > 90.0f);
}

