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
	// Vector storing instances of all possible regular room prefabs, to clone from
	vector<DungeonRoom*> RegularRooms;
	// Vector storing instances of all possible special room prefabs, to clone from
	vector<DungeonRoom*> SpecialRooms;
	// Vector storing instances of all possible boss room prefabs, to clone from
	vector<DungeonRoom*> BossRooms;

	RoomStorage();
	~RoomStorage();

	// Returns a random entrance room instance, cloned from storage
	DungeonRoom* GetRandomEntranceRoom();
	// Returns a random regular room that has the required doors (U, D, L, R) so it can be connected to a previous room as well 
	// as surrounding ones, but also has none of the exits included on the second parameter, so there's no doors that lead to nothing
	DungeonRoom* GetRandomRegularRoom(vector<char> exitsNeeded, vector<char> exitsBlacklisted);
	// Returns a random special room that has the required doors (U, D, L, R) so it can be connected to a previous room. 
	// Unlike with regular rooms, this time it can only have the required doors and no more.
	DungeonRoom* GetRandomSpecialRoom(vector<char> exitsNeeded);
	// Returns a random boss room that has the required door (U, D, L, R) so it can be connected to a previous room. 
	// Unlike with regular rooms, this time it can only have the required door and no more.
	DungeonRoom* GetRandomBossRoom(char exit);

	// Reads all room files and creates DungeonRoom instances for each of them within each vector
	void readAllRoomFiles();
	// FOR TESTING: prints out all room data to console, from all vectors
	void printRoomData(DungeonRoom*);
};