#pragma once

#include "DungeonRoom.h"

#include "vector"
#include "iostream"
using namespace std;

class DungeonFloor
{
private:
	int floor_width; // In terms of the room matrix
	int floor_height; // In terms of the room matrix
	vector<vector<DungeonRoom>> floorLayout; // Room matrix
public:
	DungeonFloor(int floorW, int floorH);
	~DungeonFloor();
	virtual void render() const;
	virtual void update();

	void GenerateFloor();
};