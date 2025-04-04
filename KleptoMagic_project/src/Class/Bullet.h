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
#include "../Class/ImageWithFrames.h"
#include "BulletStats.h"
#include "HomingComponent.h"
class Bullet 
{
public:
	
	Bullet();
	~Bullet();
	
	
	void update();
	void render();
	void reset();
	void hit(int index);
	void pressed();
	void collided(ecs::entity_t e);
	void addComponent(int i);
	void checkComponent(int i, ecs::entity_t e);
	void changeAttSpeedCapFlat(float i) { attSpeedCapFlat += i; }
	void changeAttSpeedCapMul(float i) { attSpeedCapMul += i; }
private:
	VirtualTimer* _tim;
	constexpr static uint8_t _max_bullets = 20;
	Texture* tex = &sdlutils().images().at("star");
	void shoot();
	std::array<bool, 20> componentes;
	float attSpeedCapFlat=250;
	float attSpeedCapMul = 1;
};

