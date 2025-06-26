#pragma once

#include "../sdlutils/SDLUtils.h"
#include "../utils/Singleton.h"
#include "../utils/Vector2D.h"
#include "../ecs/Component.h"
#include "../ecs/Manager.h"
#include "../render/Image.h"
#include "../room/PickableCMP.h"
#include "../room/ObjectInfo.h"


using ecs::Manager;

enum ObjectNames {
	OBJECT_PICKABLE
};

class ObjectUtils : public Singleton<ObjectUtils> {
	friend Singleton<ObjectUtils>;

protected:
	ObjectUtils();
	virtual ~ObjectUtils();

	Manager* _mngr;

	bool init(Manager* mngr);

	void spawn_UPGRADE(Vector2D pos);	// modifica exclusivamente los stats del jugador

public:
	void removeAllObjects();
	bool spawnObject(ObjectNames name, Vector2D pos);	// pone el item en el mapa
	bool spawnObject(ObjectNames name, Transform tr);	// por si quieres instanciarlo mas facilmente desde un enemigo
};

