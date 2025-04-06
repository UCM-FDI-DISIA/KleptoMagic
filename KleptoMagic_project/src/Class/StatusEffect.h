#include "../ecs/Component.h"
#include <unordered_map>
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
#include "EntityStat.h"


class StatusEffect : public ecs::Component
{
	__CMPID_DECL__(ecs::cmp::STATUSEFFECT);
	int timeLeft;
	int effectStrength;
	enum Status {slow, stun, dot};
	//struct con int duración, int Potencia
	struct StatusInfo {
		float duration = 0;
		int strength = 0;
	};
	//pair de enum Status y struct de info
	std::unordered_map<Status, StatusInfo> effects;

	EntityStat* _entStat;
	
public:
	StatusEffect();
	void initComponent() override;
	void update();

	Status GetStatus();
	void SetStatus();
	void AddStatus(Status stat, int strength, float time);

	void DecreaseTime();
	void UpdateSpeed();
	~StatusEffect();
};
