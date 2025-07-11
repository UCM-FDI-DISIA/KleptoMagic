#include "ObjectUtils.h"

ObjectUtils::ObjectUtils() : _mngr(nullptr) 
{
	_storage = new ItemStorage("resources/item_data/objetos.txt");
	
}

bool ObjectUtils::init(Manager* mngr) { 
	
	_mngr = mngr;
	assert(_mngr != nullptr);
	return true; 
}

ObjectUtils::~ObjectUtils() 
{
	removeAllObjects();
	if (_storage != nullptr) {
		delete _storage;
		_storage = nullptr;
	}
}


void ObjectUtils::spawn_RAMDOM_UPGRADE(Vector2D pos) 
{
	auto upgrade = _mngr->addEntity(ecs::grp::OBJECT);
	auto s = 50.0f;
	auto tr = _mngr->addComponent<Transform>(upgrade);
	tr->init(pos, Vector2D(), s, s, 0.0f);

	Stats randomItem = _storage->getRandomItem();
	std::string textureName = randomItem.name;

	try {
		_mngr->addComponent<Image>(upgrade, &sdlutils().images().at(textureName));
	}
	catch (const std::out_of_range&) {
		// Si la textura no existe, usar "star" como respaldo
		_mngr->addComponent<Image>(upgrade, &sdlutils().images().at("star"));
	}
	_mngr->addComponent<ObjectInfo>(upgrade, randomItem);
	auto pickable= _mngr->addComponent<PickableCMP>(upgrade);
	pickable->setBulletUtils(bullets);
}

void ObjectUtils::removeAllObjects() 
{
	for (auto e : _mngr->getEntities(ecs::grp::OBJECT)) {
		_mngr->setAlive(e, false);
	}
}

void ObjectUtils::spawnObject(ObjectNames name, Vector2D pos)
{
	switch (name) {
	case OBJECT_PICKABLE:
		spawn_RAMDOM_UPGRADE(pos);
		break;
	default:
		break;
	}
}

void ObjectUtils::spawnObject(ObjectNames name, Transform tr) 
{
	Vector2D aux = tr.getPos();
	spawnObject(name, aux);
}

void  ObjectUtils::updateStorage(ItemStorage* storage) 
{
	if (_storage != nullptr) {
		delete _storage;
	}
	_storage = storage;
}

void ObjectUtils::spawnRandomItem(Vector2D pos) 
{
	spawn_RAMDOM_UPGRADE(pos);
}