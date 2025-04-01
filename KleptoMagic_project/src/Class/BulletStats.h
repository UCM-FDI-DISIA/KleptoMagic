#include "../ecs/Component.h"

class BulletStats: public ecs::Component
{
public:
	__CMPID_DECL__(ecs::cmp::BULLETSTATS);

	BulletStats();
	~BulletStats();
	void Created(int i);
	float getSpeed() { return speed*speedMul; }
	float getDamage() { return damage*damageMul; }
	float getDistance() { return distance*distanceMul; }
	float getSize() { return size*sizeMul; }
	bool getPiercing() { return piercing; }
	void changeSpeedflat(int i) { speed += i; }
	void changeSpeedMul(float i) { speedMul+=i; }
	void changeDamageflat(int i) { speed += i; }
	void changeDamageMul(float i) { speedMul += i; }
	void changeDistanceflat(int i) { distance += i; }
	void changeDistanceMul(float i) { distanceMul += i; }
	void changeSizeflat(int i) { speed += i; }
	void changeSizeMul(float i) { speedMul += i; }
	void changePiercing(bool b) { piercing = b; }
private:
	float speed;
	float speedMul=1;
	float damage;
	float damageMul=1;
	float distance;
	float distanceMul=1;
	float size;
	float sizeMul=1;
	bool piercing;
};

