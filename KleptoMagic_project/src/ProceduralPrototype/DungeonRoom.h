#pragma once
#include "./../utils/Vector2D.h"

class DungeonRoom
{
private:

public:
	DungeonRoom();
	~DungeonRoom();
	virtual void render() const;
	virtual void update();
};