#include "ObjectUtils.h"

ObjectUtils::ObjectUtils() : _mngr(nullptr) {};

bool ObjectUtils::init(Manager* mngr) { return true; }

ObjectUtils::~ObjectUtils() 
{
	removeAllObjects();
}


void ObjectUtils::spawn_UPGRADE(Vector2D pos) 
{
	auto upgrade = _mngr->addEntity(ecs::grp::OBJECT);
	auto s = 50.0f;
	auto tr = _mngr->addComponent<Transform>(upgrade);
	tr->init(pos, Vector2D(), s, s, 0.0f);
	_mngr->addComponent<Image>(upgrade, &sdlutils().images().at("star"));
	//_mngr->addComponent<ObjectInfo>(upgrade, "daño", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0);
	_mngr->addComponent<PickableCMP>(upgrade);
}

void ObjectUtils::removeAllObjects() 
{
	for (auto e : _mngr->getEntities(ecs::grp::OBJECT)) {
		_mngr->setAlive(e, false);
	}
}

bool ObjectUtils::spawnObject(ObjectNames name, Vector2D pos)
{
	switch (name) {
	case OBJECT_PICKABLE:
		spawn_UPGRADE(pos);
		return true;
		break;
	default:
		return false;
		break;
	}
}

bool ObjectUtils::spawnObject(ObjectNames name, Transform tr) 
{
	Vector2D aux = tr.getPos();
	return spawnObject(name, aux);
}