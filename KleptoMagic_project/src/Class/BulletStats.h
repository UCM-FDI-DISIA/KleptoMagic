#include "../ecs/Component.h"
#include "../sdlutils/SDLUtils.h"

class BulletStats: public ecs::Component
{
public:
	__CMPID_DECL__(ecs::cmp::BULLETSTATS);

	BulletStats();
	~BulletStats();
	void update();
	float Created();
	float getSpeed() { return speed*speedMul; }
	float getDamage() { return damage*damageMul; }
	float getDistance() { return distance*distanceMul; }
	float getSize() { return size*sizeMul; }
	bool getPiercing() { return piercing; }
	void changeSpeedflat(int i) { speed += i; refreshDuration();}
	void changeSpeedMul(float i) { speedMul+=i; refreshDuration();}
	void changeDamageflat(int i) { damage += i; }
	void changeDamageMul(float i) { damageMul += i; }
	void changeDistanceflat(int i) { distance += i; refreshDuration();}
	void changeDistanceMul(float i) { distanceMul += i; refreshDuration(); }
	void changeSizeflat(int i) { speed += i; }
	void changeSizeMul(float i) { speedMul += i; }
	void changePiercing(bool b) { piercing = b; }
	void refreshDuration() { duration = distance / speed; }
private:
	float speed;
	float speedMul=1;
	float damage;
	float damageMul=1;
	float distance;
	float distanceMul=1;
	float size;
	float sizeMul=1;
	float duration;
	bool piercing;
	VirtualTimer* _tim= new VirtualTimer();
};

