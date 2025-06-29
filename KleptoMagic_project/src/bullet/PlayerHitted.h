#include "../ecs/Component.h"
#include "../sdlutils/SDLUtils.h"
#include "../ecs/Manager.h"
#include <array>

class PlayerHitted : public ecs::Component
{
public:
	__CMPID_DECL__(ecs::cmp::PLAYERHITTED);

	PlayerHitted() { hitted = false; }
	~PlayerHitted() { }
	bool AddPlayer()
	{
		if (!hitted)
		{
			hitted = true;
			return true;
		}
		else { return false; }
	}

private:

	bool hitted;

};

