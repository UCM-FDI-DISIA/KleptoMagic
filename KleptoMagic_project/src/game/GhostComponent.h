#include "../Class/Transform.h"


#pragma once
class GhostComponent : public ecs::Component
{
protected:
	Transform* _ghostTransform;
	int _teleRadius;
	
	void teleport();
	bool canTeleport();
public:
	GhostComponent(Transform* transform, int teleRadius);
	virtual ~GhostComponent();
	void initComponent() override;
	void update() override;

};

