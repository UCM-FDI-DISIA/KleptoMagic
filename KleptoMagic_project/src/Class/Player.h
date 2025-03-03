#include "Entity.h"
#include "vector"
#include "string"
#include "../utils/Vector2D.h"
#include "EventHandler.h"
#include "MovementCtrl.h"
class Game;
class Player : public Entity, public EventHandler, public MovementCtrl //hace falta que herede de Movement controller en verdad? o puede simplemente incluirlo?
{
	float attackcooldown;
	float abilitycooldown;
	float procMultiplayer;
	std::vector <std::pair<std::string, int>> itemList;
	int parallelshots = 0;
	int shotgunshots = 0;
	Game* game;

public:
	Player(Game* newgame,float atackcooldown, float skillcooldown, float multi, Vector2D velocity, float life, float moveSpeed, float attack, float attacksped, float armor, Vector2D position, PlayState* playstate);
	void update();
	//virtual void attack();
	//virtual void skill();
	void GetItem(std::string item);
	void RemoveItem(std::string item);
	//void ApplyProjectileModifiers();
	//void CalculateStatusPostItem();
	~Player();
	void render() const override;
	void handleEvent(const SDL_Event& e);
};





