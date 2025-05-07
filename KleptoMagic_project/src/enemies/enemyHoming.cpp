#include "enemyHoming.h"
#include <cmath>
#include "../bullet/BulletStats.h"
enemyHoming::enemyHoming()
{

}

enemyHoming::~enemyHoming()
{

}

void enemyHoming::initComponent()
{
	_tr = game().getMngr()->getComponent<Transform>(this->_ent);
}

void enemyHoming::update()
{
	auto mngr = game().getMngr();
	float distance = 0;

		auto* stats = mngr->getComponent<BulletStats>(this->_ent);
		auto playerTR = mngr->getComponent<Transform>(mngr->getHandler(ecs::hdlr::PLAYER));
		Vector2D distanceE = Vector2D(playerTR->getPos().getX() + playerTR->getWidth() / 2, playerTR->getPos().getY() + playerTR->getHeight() / 2) - Vector2D(_tr->getPos().getX() + (stats->getSize() / 2), _tr->getPos().getY() + (stats->getSize() / 2));
		float distanceHypo = sqrt(pow(distanceE.getX(), 2) + pow(distanceE.getY(), 2));
		if (distance == 0 || distanceHypo < distance)
		{
			distance = distanceHypo;
			_tr->getVel().set(distanceE.normalize() * stats->getSpeed());
		}

}
