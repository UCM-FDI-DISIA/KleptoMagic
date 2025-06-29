#pragma once

#include "../sdlutils/SDLUtils.h"
#include "../utils/Singleton.h"
#include "../utils/Vector2D.h"
#include "../ecs/Component.h"
#include "../ecs/Manager.h"
#include "../render/Image.h"
#include "../room/PickableCMP.h"
#include "../room/ItemStorage.h"
#include "../bullet/BulletUtils.h"

using ecs::Manager;

enum ObjectNames {
	RANDOM,
	OBJECT_PICKABLE
};

class ObjectUtils : public Singleton<ObjectUtils> {
	friend Singleton<ObjectUtils>;

protected:
	ObjectUtils();
	virtual ~ObjectUtils();

	Manager* _mngr;
	ItemStorage* _storage;
	BulletUtils* bullets;
	bool init(Manager* mngr);

	void spawn_RAMDOM_UPGRADE(Vector2D pos);	// modifica exclusivamente los stats del jugador

public:
	void removeAllObjects();
	void updateStorage(ItemStorage* storage);
	void spawnObject(ObjectNames name, Vector2D pos);	// pone el item en el mapa
	void spawnObject(ObjectNames name, Transform tr);	// por si quieres instanciarlo mas facilmente desde un enemigo
	void spawnRandomItem(Vector2D pos);
	void setBulletUtils(BulletUtils* b) { bullets = b; }
};

