# pragma once
#include <SDL.h>
#include "../utils/Vector2D.h"
#include "../ecs/Component.h"
#include "../ecs/Transform.h"
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
#include "../bullet/DestroyOnBorder.h"
#include "../ecs/Game.h"
#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/NewInputHandler.h"
#include "EnemyHitted.h"
#include "../render/ImageWithFrames.h"
#include "../bullet/BulletStats.h"
#include "../bullet/HomingComponent.h"
#include "../map/DungeonFloor.h"
#include "../player/StatusEffect.h"
#include "../room/TileCollisionChecker.h"
class BulletUtils 
{
public:
	
	BulletUtils();
	~BulletUtils();
	
	
	void update();
	void render();
	void reset();
	void hit(Entity* target,Entity* bullet);
	void pressed();
	void enemyShoot(Transform* enemyTR,int i);
	void BossManyDirectinons(Transform* bossTR, Vector2D v);
	void collided(ecs::entity_t e);
	void addComponent(int i);
	void checkComponent(int i, ecs::entity_t e);
	void changeSpeedflat(int i) { bulStat->changeSpeedflat(i); }
	void changeSpeedMul(float i) { bulStat->changeSpeedMul( i); }
	void changeDamageflat(int i) { bulStat->changeDamageflat( i); }
	void changeDamageMul(float i) { bulStat->changeDamageMul(i); }
	void changeDistanceflat(int i) { bulStat->changeDistanceflat( i); }
	void changeDistanceMul(float i) { bulStat->changeDistanceMul( i); }
	void changeSizeflat(int i) { bulStat->changeSizeflat( i); }
	void changeSizeMul(float i) { bulStat->changeSizeMul( i); }
	void changePiercing(bool b) { bulStat->changePiercing( b); }
	void refreshDuration() { ; }
	void changeAttSpeedCapFlat(float i) { attSpeedCapFlat += i; }
	void changeAttSpeedCapMul(float i) { attSpeedCapMul += i; }
	void setDungeonFloor(DungeonFloor* floor) { _dungeonfloor = floor; }
	void addBullets(int i) { bulStat->changeBullets(i); }
private:
	VirtualTimer* _tim;
	constexpr static uint8_t _max_bullets = 20;
	Texture* tex = &sdlutils().images().at("player_bullet");
	void shoot();
	void MultiShot(Vector2D v, BulletStats* stat, bool fromPlayer,Transform* tr = nullptr);
	void IndividualShotP(Vector2D v);
	void IndividualShotH(Vector2D v, Transform* tr);
	std::array<bool, 20> componentes;
	float attSpeedCapFlat=250;
	float attSpeedCapMul = 1;
	std::string player;
	BulletStats* bulStat = new BulletStats();
	DungeonFloor* _dungeonfloor = nullptr;
};

