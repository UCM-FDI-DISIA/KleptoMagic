#pragma once

#include "DungeonRoom.h"

#include "vector"
#include "iostream"
using namespace std;

class RoomStorage
{
private:
	vector<DungeonRoom*> EntranceRooms;
	vector<DungeonRoom*> RegularRooms;
	vector<DungeonRoom*> BossRooms;
public:
	RoomStorage();
	~RoomStorage();

	void readAllRoomFiles();
	void printRoomData(DungeonRoom*);
};