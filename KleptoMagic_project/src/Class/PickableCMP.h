#ifndef PICK_COMPONENT_C
#define PICK_COMPONENT_C

#include "../ecs/Component.h"
#include "../Class/Transform.h"


#pragma once
class PickableCMP : public ecs::Component
{
private:
	//auto* _ghostTransform;


	bool visible = true;

public:
	void initComponent() override;


};

#endif // PICK_COMPONENT_C