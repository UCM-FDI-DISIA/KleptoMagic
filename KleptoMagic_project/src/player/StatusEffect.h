#include "../ecs/Component.h"
#include <unordered_map>
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
#include "../ecs/EntityStat.h"


class StatusEffect : public ecs::Component
{
	
	

public:
	enum Status { slow, stun, dot };
	__CMPID_DECL__(ecs::cmp::STATUSEFFECT);
	StatusEffect();
	void initComponent() override;
	void update();

	Status GetStatus();
	void SetStatus();
	void AddStatus(Status stat, int strength, float time);

	void DecreaseTime();
	void UpdateSpeed();
	~StatusEffect();
private:
	int timeLeft;
	int effectStrength;
	
	//struct con int duración, int Potencia
	struct StatusInfo {
		float duration = 0;
		int strength = 0;
	};
	//pair de enum Status y struct de info
	std::unordered_map<Status, StatusInfo> effects;

	EntityStat* _entStat;
};