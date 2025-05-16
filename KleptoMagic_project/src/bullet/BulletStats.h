#include "../ecs/Component.h"
#include "../sdlutils/SDLUtils.h"
#include "../ecs/EntityStat.h"
#include "../ecs/Manager.h"

class BulletStats: public ecs::Component
{
public:
	__CMPID_DECL__(ecs::cmp::BULLETSTATS);

	BulletStats();
	~BulletStats();
	void initComponent() override;
	void update();
	float Created(std::string s);
	void enemyStats(int i);
	float getSpeed() { return speed*speedMul; }
	float getDamage() { return damage*damageMul; }
	float getDistance() { return distance*distanceMul; }
	float getSize() { return size*sizeMul; }
	bool getPiercing() { return piercing; }
	int getBull() { return bullets; }
	float getSlowT() { return slowT; }
	float getSlowS() { return slowS; }
	float getStun() { return stunT; }
	float getDotT() { return DotT; }
	float getDotS() { return DotS; }
	void changeSpeedflat(int i) { speed += i; refreshDuration();}
	void changeSpeedMul(float i) { speedMul+=i; refreshDuration();}
	void changeDamageflat(int i) { damage += i; }
	void changeDamageMul(float i) { damageMul += i; }
	void changeDistanceflat(int i) { distance += i; refreshDuration();}
	void changeDistanceMul(float i) { distanceMul += i; refreshDuration(); }
	void changeSizeflat(int i) { size += i; }
	void changeSizeMul(float i) { sizeMul += i; }
	void changePiercing(bool b) { piercing = b; }
	void changeBullets(int b) { bullets += b; }
	void refreshDuration() { duration = distance / speed; }
	//void setStats(float spe,float dmg,float dist,float siz,bool pierc);
	void refreshStats(float spe,float dmg,float dist,float siz,bool pierc, int bull, float slowt,float slows,float stunt,float dott,float dots);

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
	int bullets = 1;
	float slowT=0; 
	float slowS=0; 
	float stunT=0;
	float DotT=0;
	float DotS=0;
	VirtualTimer* _tim= new VirtualTimer();
	EntityStat* _eStat;
};

