#ifndef GHOST_COMPONENT_C
#define GHOST_COMPONENT_C


#include "../ecs/Transform.h"
#include <random>
#include <chrono>


const int DEFAULT_TELE_RADIUS = 100;
const int DEFAULT_COOLDOWN_TIMER = 3;
#pragma once
class GhostComponent : public ecs::Component
{
protected:
	Transform* _ghostTransform;
	Transform* _targetTransform;
	std::default_random_engine rndGenerator;
	std::uniform_real_distribution<float> rndDistributor;
	std::chrono::steady_clock::time_point lastAttackTime;

	int _teleRadius = DEFAULT_TELE_RADIUS;
	int _cooldownTime = DEFAULT_COOLDOWN_TIMER;
	bool visible = true;

	void teleportRndPosition();			// Teleports the ghost
	
public:
	__CMPID_DECL__(ecs::cmp::GHOSTCMP);

	// inicializadores
	GhostComponent(int rad = DEFAULT_TELE_RADIUS, int timer = DEFAULT_COOLDOWN_TIMER);	// Constructor with radius and timer parameters
	virtual ~GhostComponent();	// Destructor
	void initComponent() override;
	void update() override;

};

#endif // GHOST_COMPONENT_C