#include "../ecs/Component.h"
#include "../sdlutils/SDLUtils.h"
#include "../ecs/Manager.h"
#include <array>

class EnemyHitted: public ecs::Component
{
public:
	__CMPID_DECL__(ecs::cmp::ENEMYHITTED);

	EnemyHitted() { index = 0; }
	~EnemyHitted() { for (auto itc : list) 
	{
		delete itc;
	} 
	}
	bool AddEnemy(ecs::Entity* e)
	{
		if (contains(e)) { return false; }
		else
		{
			list.push_back(e);
			return true;
		}
	}

private:
	
	std::vector<ecs::Entity*> list;

	int index;
	bool contains(ecs::Entity* e)
	{
		bool contain = false;
		int i = 0;
		while(!contain&&i<list.size())
		{
			if (e == list[i]) { contain = true; }
		 i++; 
		}
		return contain;
	}
};

