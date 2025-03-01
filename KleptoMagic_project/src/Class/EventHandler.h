//#include "checkML.h"
//#include "gameList.h"
#include "SceneObject.h"

#include <SDL.h>


#pragma once
class EventHandler
{
public:

	virtual void handleEvent(const SDL_Event& e) = 0;
	virtual ~EventHandler() = default;




};

