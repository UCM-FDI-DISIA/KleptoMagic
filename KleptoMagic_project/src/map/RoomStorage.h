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

	// Constructor for RoomStorage. Should be called at the start of the game. 
	// Its purpose is to store a clean slate of every room that is defined in files.
	RoomStorage();
	~RoomStorage();

	// GENERATION ONLY: 
	// Returns a random entrance room instance, cloned from storage
	DungeonRoom* GetRandomEntranceRoom();
	// GENERATION ONLY: 
	// Returns a random regular room that has the required doors (U, D, L, R) so it can be connected to a previous room as well 
	// as surrounding ones, but also has none of the exits included on the second parameter, so there's no doors that lead to nothing
	DungeonRoom* GetRandomRegularRoom(vector<char> exitsNeeded, vector<char> exitsBlacklisted);
	// GENERATION ONLY: 
	// Returns a random special room that has the required doors (U, D, L, R) so it can be connected to a previous room. 
	// Unlike with regular rooms, this time it can only have the required doors and no more.
	DungeonRoom* GetRandomSpecialRoom(vector<char> exitsNeeded);
	// GENERATION ONLY: 
	// Returns a random boss room that has the required door (U, D, L, R) so it can be connected to a previous room. 
	// Unlike with regular rooms, this time it can only have the required door and no more.
	DungeonRoom* GetRandomBossRoom(vector<char> exitsNeeded);

	// Reads all room files and creates clean slate DungeonRoom instances for each of them within each of the storage vectors.
	void readAllRoomFiles();
#ifdef _DEBUG
	// DEBUG ONLY: prints out all room data to console, from all storage vectors.
	void printRoomData(DungeonRoom* room);
#endif

};