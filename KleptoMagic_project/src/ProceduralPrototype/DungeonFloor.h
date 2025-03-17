#pragma once

#include "DungeonRoom.h"
#include "RoomStorage.h"

#include "vector"
#include "iostream"
using namespace std;

class DungeonFloor
{
private:
	// In terms of the room matrix
	int floor_width; 
	// In terms of the room matrix
	int floor_height; 
	// Room matrix
	vector<vector<DungeonRoom*>> floorLayout;
	// Pointer to room storage
	RoomStorage* roomstorage;
public:
	DungeonFloor(RoomStorage* roomstorage);
	~DungeonFloor();
	virtual void render() const;
	virtual void update();
	void GenerateFloor();
	vector<char> CheckSpaceAroundRoom(int x, int y);
	vector<char> ExitsToFillForSpace(int x, int y);
	void PrintFloorLayout_Simple();
	void PrintFloorLayout_Detailed();
};