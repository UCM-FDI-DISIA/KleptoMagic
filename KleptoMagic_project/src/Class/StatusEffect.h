
class StatusEffect
{
	int timeLeft;
	int effectStrength;
	enum effectType {health,damage,range,movementSpeed,attackSpeed,bulletSpeed,maxHealth,Shield};
	int effectnumber;
	
public:
	StatusEffect(int time, int strenght, int effectTypenumber);
	int GetType();
	void DecreaseTime();
	~StatusEffect();
};
