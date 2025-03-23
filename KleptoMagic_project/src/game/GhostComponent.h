#include "../Class/Transform.h"


const int DEFAULT_TELE_RADIUS = 100;
const int DEFAULT_TIMER = 5;
#pragma once
class GhostComponent : public ecs::Component
{
protected:
	Transform* _ghostTransform;
	Transform* _playerTransform;
	int _teleRadius = DEFAULT_TELE_RADIUS;
	int _teleTimer = DEFAULT_TIMER;

	void teleport();
	bool canTeleport();
	
public:
	__CMPID_DECL__(ecs::cmp::GHOSTCMP);

	GhostComponent(int rad);
	GhostComponent(int rad, int timer);
	virtual ~GhostComponent();
	void initComponent() override;
	void update() override;

};

