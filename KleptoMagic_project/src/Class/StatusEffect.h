#include "../ecs/Component.h"
#include <unordered_map>


class StatusEffect : public ecs::Component
{
	__CMPID_DECL__(ecs::cmp::STATUSEFFECT);
	int timeLeft;
	int effectStrength;
	enum Status {slow, stun, dot};
	//struct con int duración, int Potencia
	struct StatusInfo {
		int duration;
		int strength;
	};
	//pair de enum Status y struct de info
	std::unordered_map<Status, StatusInfo> effects;
	
public:
	StatusEffect(int time, int strenght, int effectTypenumber);
	//getter generico
	//setter generico
	//update
	Status GetStatus();
	void SetStatus();
	void Updaate();
	~StatusEffect();
};
