#include <SDL.h>
#include "../utils/Vector2D.h"
#include "../ecs/Component.h"
#include "Transform.h"
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
class Bullet : public ecs::Component
{
public:
	__CMPID_DECL__(ecs::cmp::BULLET);

	~Bullet();
	struct projectile {
		bool used = false;
		Vector2D pos = { 0,0 };
		Vector2D vel = { 0,0 };
		int width = 10;
		int height = 20;
		float rot = 0.0f;
		float distance=0.0f;

	};
	constexpr static uint8_t _max_bullets = 20;
	typedef std::array<projectile, _max_bullets> bullets_array_t;
	typedef bullets_array_t::iterator iterator;
	void initComponent();
	void update();
	void render();
	void reset();
	void hit(int index);
	void pressed();
	iterator begin() {
		return _bullets.begin();
	}
	iterator end() {
		return _bullets.end();
	}
	bullets_array_t getBullets() { return _bullets; }
	
private:
	
	bullets_array_t _bullets;
	Transform* _tr;
	float speed=0.0f;
	void shoot(int index);


};

