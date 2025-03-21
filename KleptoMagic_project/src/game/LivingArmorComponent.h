#include "../Class/Transform.h"
#include "../ecs/Component.h"
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
# define M_PIl          3.141592653589793238462643383279502884L /* pi */

#pragma once

class LivingArmorComponent : public ecs::Component {
protected:
	Transform* _armorTransform; // Transform de la entidad
	Transform* _playerTransform; // Transform del jugador
	Vector2D currentDir_;       // Dirección actual de movimiento
	float movementSpeed_;       // Velocidad de movimiento
	float directionChangeTime_; // Cada cuánto cambia dirección (segundos)
	float timer_;               // Temporizador interno

public:
	LivingArmorComponent(float speed, float dirChangeTime);
	virtual ~LivingArmorComponent();
	void initComponent() override;
	void update() override;

	bool isVulnerable(const Vector2D& attackerPosition) const;

};