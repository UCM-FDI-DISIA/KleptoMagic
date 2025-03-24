#pragma once

#include "DungeonRoom.h"
#include "RoomStorage.h"

#include "vector"
#include "iostream"
using namespace std;

struct roomPos {
	int x;
	int y;
	bool operator==(roomPos const& other) const
	{
		return (this->x == other.x && this->y == other.y);
	}
	bool operator!=(roomPos const& other) const
	{
		return (this->x != other.x || this->y != other.y);
	}
}; 


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
	// Used to indicate when a fail has happened during generation
	bool badGen;
	DungeonFloor(int minWidth, int minHeight, int maxWidth, int maxHeight, int numRooms, RoomStorage* roomstorage);
	~DungeonFloor();
	virtual void render() const;
	virtual void update();
	void GenerateFloor(int minWidth, int minHeight, int maxWidth, int maxHeight, int numRooms);
	vector<char> CheckSpaceAroundRoom(int x, int y);
	vector<char> ExitsToFillForSpace(int x, int y);
	void LinkExitsAtPosition(int x, int y, vector<char> exits);
	void addPos(roomPos pos, vector<roomPos>& locations);
	void PrintFloorLayout_Simple();
	void PrintFloorLayout_Detailed();
};