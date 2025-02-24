#pragma once

#include "DungeonRoom.h"

#include "vector"
#include "iostream"
using namespace std;

class DungeonFloor
{
private:
	int floor_width;
	int floor_height;
	vector<vector<int>> floorLayout;
public:
	DungeonFloor(int floorW, int floorH);
	~DungeonFloor();
	virtual void render() const;
	virtual void update();
};