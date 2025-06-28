#pragma once
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
	void explosionStats(int damage);
	void enemyStats(int i);
	float getSpeed() { return speed*speedMul; }
	float getDamage() { return damage*damageMul; }
	float getDistance() { return distance*distanceMul; }
	float getSize() { return size*sizeMul; }
	bool getPiercing() { return piercing; }
	bool getExplode() { return explode; }
	int getBull() { return bullets; }
	float getSlowT() { return slowT; }
	float getSlowS() { return slowS; }
	float getStun() { return stunT; }
	float getDotT() { return DotT; }
	float getDotS() { return DotS; }
	float getDuration() { return duration; }
	float getSartTime() { return startTime; }
	void changeSpeedflat(int i) { speed += i; refreshDuration();}
	void changeSpeedMul(float i) { speedMul+=i; refreshDuration();}
	void changeDamageflat(int i) { damage += i; }
	void changeDamageMul(float i) { damageMul += i; }
	void changeDistanceflat(int i) { distance += i; refreshDuration();}
	void changeDistanceMul(float i) { distanceMul += i; refreshDuration(); }
	void changeSizeflat(int i) { size += i; }
	void changeSizeMul(float i) { sizeMul += i; }
	void changePiercing(bool b) { piercing = b; }
	void changeExplode(bool b) { explode = b; }
	void changeBullets(int b) { bullets += b; }
	void refreshDuration() { duration = distance / speed; }
	
	//void setStats(float spe,float dmg,float dist,float siz,bool pierc);
	void refreshStats(float spe,float dmg,float dist,float siz,bool pierc,bool explode, int bull, float slowt,float slows,float stunt,float dott,float dots);

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
	bool explode;
	int bullets = 1;
	float slowT=0; 
	float slowS=0; 
	float stunT=0;
	float DotT=0;
	float DotS=0;
	float startTime;
	EntityStat* _eStat;
};

