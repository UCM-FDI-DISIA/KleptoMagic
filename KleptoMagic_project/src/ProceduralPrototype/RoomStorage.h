#pragma once

#include "DungeonRoom.h"

#include "vector"
#include "iostream"
using namespace std;

class RoomStorage
{
private:
public:

	// Vector storing instances of all possible entrance room prefabs, to clone from
	vector<DungeonRoom*> EntranceRooms;
	// Vector storing instances of all possible entrance room prefabs, to clone from
	vector<DungeonRoom*> RegularRooms;
	// Vector storing instances of all possible entrance room prefabs, to clone from
	vector<DungeonRoom*> BossRooms;

	RoomStorage();
	~RoomStorage();

	// Reads all room files and creates DungeonRoom instances for each of them within each vector
	void readAllRoomFiles();
	// FOR TESTING: prints out all room data to console, from all vectors
	void printRoomData(DungeonRoom*);
};