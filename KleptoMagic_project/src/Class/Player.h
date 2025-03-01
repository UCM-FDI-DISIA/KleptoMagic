#include "Entity.h"
#include "vector"
#include "string"
#include "../utils/Vector2D.h"

class Player : public Entity
{
	float attackcooldown;
	float abilitycooldown;
	float procMultiplayer;
	std::vector <std::pair<std::string, int>> itemList;
	int parallelshots = 0;
	int shotgunshots = 0;

public:
	Player(float atackcooldown, float skillcooldown, float multi, Vector2D velocity, float life, float moveSpeed, float attack, float attacksped, float armor, Vector2D position, PlayState* playstate);
	//virtual void attack();
	//virtual void skill();
	void GetItem(std::string item);
	void RemoveItem(std::string item);
	void ApplyProjectileModifiers();
	void CalculateStatusPostItem();
	~Player();
};





