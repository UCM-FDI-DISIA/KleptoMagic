//#include "Player.h"
//#include "../utils/Vector2D.h"
//#include "Entity.h"
//#include "Game.h"
//Player::Player(Game* gam,float atackcooldown, float skillcooldown, float multi, Vector2D velocity, float life, float moveSpeed, float attack, float attacksped, float armor, Vector2D position, PlayState* playstate) : Entity(velocity, life, moveSpeed, attack, attacksped, armor, position, playstate) 
//{
//	game = gam;
//	attackcooldown = atackcooldown;
//	abilitycooldown = skillcooldown;
//	procMultiplayer = multi;
//	speed = { 0, 0 };
//}
//
//void Player::update() {
//
//	//Actualizar su movimiento respecto su input y el juego (cosas que le muevan externas al input)
//	//esto siempre deberia ir despues del input por lo que solo hay que sumarle el movimiento aplicado del escenario no remplazarlo
//	//Mover al jugador
//	Move(position, speed, 7 /*float de stat de velocidad*/);
//
//}
//void Player::GetItem(std::string item)
//{
//	int i = 0;
//	bool encontrado = false;
//	while (!encontrado && i < itemList.size())
//	{
//		if (itemList[i].first == item)
//		{
//			itemList[i].second++;
//			encontrado = true;
//		}
//		i++;
//	}
//	if (!encontrado)
//	{
//		itemList.emplace_back(item, 1);
//	}
//}
//void Player::RemoveItem(std::string item)
//{
//	int i = 0;
//	bool encontrado = false;
//	while (!encontrado && i < itemList.size())
//	{
//		if (itemList[i].first == item && itemList[i].second > 1)
//		{
//			itemList[i].second--;
//			encontrado = true;
//		}
//		else if (itemList[i].first == item)
//		{
//			itemList.erase(itemList.begin() + i);
//			encontrado = true;
//		}
//		i++;
//	}
//}
//
//Player::~Player()
//{
//}
//
//void Player::render() const
//{
//	Texture* texture = game->getTexture(Game::PLAYER);
//	texture->render({ (int)position.getX(), (int)position.getY(), (int)width, (int)height });
//}
//
//
////Deberia de ir antes del update y ponerle el vector base de a donde se mueve y 
////luego se modificaria o cambiaria en el update en base a knockback, stun, slow recividos etc.
////Da igual que no se aplique la speed aqui porque excepto el knockback que lo sobreescribe todo
////son multiplicadores y el orden de los factores no ateran el producto 
////Espera no tendria que hacer una excepcion para el knockback si aplico la speed luego???
////Maybe lo revierto al antiguo que aplicaba la speed aqui para poder usar el move para el knockback
//void Player::handleEvent(const SDL_Event& e) {
//	//speed = /*vector2D salido del input*/{ 1,0 } ;
//	speed = game->getInputManager()->MovementVector;
//#ifdef DEBUG
//	std::cout << speed.magnitude() << std::endl;
//#endif // DEBUG
//	
//}
//