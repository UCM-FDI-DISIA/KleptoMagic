# pragma once
#include <SDL.h>
#include "../utils/Vector2D.h"
#include "../ecs/Component.h"
#include "Transform.h"
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
#include "../Class/DestroyOnBorder.h"
#include "Game.h"
#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/NewInputHandler.h"
#include "../Class/ImageWithFrames.h"
#include "BulletStats.h"
#include "HomingComponent.h"
#include "../map/DungeonFloor.h"
#include "../Class/TileCollisionChecker.h"
class BulletUtils 
{
public:
	
	BulletUtils();
	~BulletUtils();
	
	
	void update();
	void render();
	void reset();
	void hit(int index);
	void pressed();
	void enemyShoot(Transform* enemyTR,int i);
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
	Texture* tex = &sdlutils().images().at("star");
	void shoot();
	void MultiShotP(Vector2D v);
	void IndividualShotP(Vector2D v);
	std::array<bool, 20> componentes;
	float attSpeedCapFlat=250;
	float attSpeedCapMul = 1;
	std::string player;
	BulletStats* bulStat = new BulletStats();
	DungeonFloor* _dungeonfloor = nullptr;
};

