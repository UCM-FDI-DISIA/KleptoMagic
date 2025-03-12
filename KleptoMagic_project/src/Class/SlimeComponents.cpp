#include "../ecs/Component.h"
#include "../ecs/ecs.h"
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
#include "../ecs/ecs_defs_example.h"
namespace ecs 
{
	class SlimeVectorComponent : public Component
	{
		float direcionX, direcionY;
		void CreateVector(float playerX,float playerY,float enemyX,float enemyY)
		{
			direcionX = playerX - enemyX;
			direcionY = playerY - enemyY;
			float lenght = std::sqrt(direcionX * direcionX + direcionY * direcionX);
			if (lenght != 0) { direcionX / lenght; direcionY / lenght; }
		}
	};
	class SlimeMovementComponent : public Component 
	{
		void update() override 
		{
			auto vector = static_cast<SlimeVectorComponent*>(_ent->getMngr()->getComponent<SlimeVectorComponent>(_ent));

			if(vector) 
			{

			}
		}
	};
}
