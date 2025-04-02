#include "HomingComponent.h"
#include <cmath>
#include "BulletStats.h"
HomingComponent::HomingComponent()
{

}

HomingComponent::~HomingComponent()
{

}

void HomingComponent::initComponent()
{
	_tr = game().getMngr()->getComponent<Transform>(this->_ent);
}

void HomingComponent::update()
{
	auto mngr = game().getMngr();
	float distance = 0;
	for (auto enemy: mngr->getEntities(ecs::grp::ENEMY))
	{
		auto* stats = mngr->getComponent<BulletStats>(this->_ent);
		auto enemyTR = mngr->getComponent<Transform>(enemy);
		Vector2D distanceE = Vector2D(enemyTR->getPos().getX()+enemyTR->getWidth()/2, enemyTR->getPos().getY() + enemyTR->getHeight() / 2) - Vector2D(_tr->getPos().getX()+(stats->getSize()/2), _tr->getPos().getY()+(stats->getSize()/2));
		float distanceHypo = sqrt( pow(distanceE.getX(), 2) + pow(distanceE.getY(), 2));
		if(distance==0||distanceHypo<distance)
		{
			distance = distanceHypo;
			
			_tr->getVel().set(distanceE.normalize() *stats->getSpeed());
		}

	}
}
