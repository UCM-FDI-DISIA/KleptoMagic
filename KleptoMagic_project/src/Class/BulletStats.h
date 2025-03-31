#include "EntityStat.h"
#include "../ecs/Component.h"

class BulletStats: public ecs::Component
{
public:
	__CMPID_DECL__(ecs::cmp::BULLETSTATS);

	BulletStats();
	~BulletStats();
	void Created(int i);
	float getSpeed() { return speed; }
	float getDamage() { return damage; }
	float getDistance() { return distance; }
	float getSize() { return size; }
	void changeSpeedflat(int i) { speed += i; }
	void changeDamageflat(int i) { speed += i; }
	void changeDistanceflat(int i) { speed += i; }
	void changeflat(int i) { speed += i; }
private:
	float speed;
	float damage;
	float distance;
	float size;
};

