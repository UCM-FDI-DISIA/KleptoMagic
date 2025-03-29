# pragma once
#include <SDL.h>
#include "../utils/Vector2D.h"
#include "../ecs/Component.h"
#include "Transform.h"
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
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
	void generateBullets();
	
	
private:
	constexpr static uint8_t _max_bullets = 20;

	std::array<ecs::entity_t,_max_bullets> _bullets;
	std::array<Transform, _max_bullets> _bulletsTR;
	float speed=0.0f;
	void shoot(int index);


};

