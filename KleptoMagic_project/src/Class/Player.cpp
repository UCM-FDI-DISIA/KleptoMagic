#include "Player.h"
#include "../utils/Vector2D.h"
#include "Entity.h"
Player::Player(float atackcooldown, float skillcooldown, float multi, Vector2D velocity, float life, float moveSpeed, float attack, float attacksped, float armor, Vector2D position, PlayState* playstate) : Entity(velocity, life, moveSpeed, attack, attacksped, armor, position, playstate) 
{
	attackcooldown = atackcooldown;
	abilitycooldown = skillcooldown;
	procMultiplayer = multi;
}
void Player::GetItem(std::string item)
{
	int i = 0;
	bool encontrado = false;
	while (!encontrado && i < itemList.size())
	{
		if (itemList[i].first == item)
		{
			itemList[i].second++;
			encontrado = true;
		}
		i++;
	}
	if (!encontrado)
	{
		itemList.emplace_back(item, 1);
	}
}
void Player::RemoveItem(std::string item)
{
	int i = 0;
	bool encontrado = false;
	while (!encontrado && i < itemList.size())
	{
		if (itemList[i].first == item && itemList[i].second > 1)
		{
			itemList[i].second--;
			encontrado = true;
		}
		else if (itemList[i].first == item)
		{
			itemList.erase(itemList.begin() + i);
			encontrado = true;
		}
		i++;
	}
}
