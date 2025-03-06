
#include "RoomStorage.h"
#include "DungeonRoom.h"
#include "filesystem"
#include <random>

namespace fs = std::filesystem;

RoomStorage::RoomStorage()
{
	readAllRoomFiles();
	/*
	for (auto i : EntranceRooms) {
		printRoomData(i);
	}
	for (auto i : RegularRooms) {
		printRoomData(i);
	}
	for (auto i : BossRooms) {
		printRoomData(i);
	}
	*/
}

RoomStorage::~RoomStorage()
{

}

DungeonRoom* RoomStorage::GetRandomEntranceRoom() {
	int minNum = 0;
	int maxNum = EntranceRooms.size() - 1;
	srand((unsigned int)time(NULL));
	int randomRoom = rand() % (maxNum - minNum + 1) + minNum;
	return new DungeonRoom{ *EntranceRooms[randomRoom] };
}
DungeonRoom* RoomStorage::GetRandomRegularRoom(char exit, vector<char> noExits) {
	vector<DungeonRoom*> results;
	for (auto i : RegularRooms) {
		bool hasCorrectEntrance = false;
		if (exit == 'U' && i->hasExitUp()) hasCorrectEntrance = true;
		else if (exit == 'D' && i->hasExitDown()) hasCorrectEntrance = true;
		else if (exit == 'L' && i->hasExitLeft()) hasCorrectEntrance = true;
		else if (exit == 'R' && i->hasExitRight()) hasCorrectEntrance = true;

		bool hasNoWrongExits = true;
		for (auto j : noExits) {
			if (j == 'U' && i->hasExitUp()) hasCorrectEntrance = false;
			else if (j == 'D' && i->hasExitDown()) hasCorrectEntrance = false;
			else if (j == 'L' && i->hasExitLeft()) hasCorrectEntrance = false;
			else if (j == 'R' && i->hasExitRight()) hasCorrectEntrance = false;
		}
		
		if (hasCorrectEntrance && hasNoWrongExits) {
			results.push_back(i);
		}
	}

	int minNum = 0;
	int maxNum = results.size() - 1;
	srand((unsigned int)time(NULL));
	int randomRoom = rand() % (maxNum - minNum + 1) + minNum;
	return new DungeonRoom{ *results[randomRoom] };
}

DungeonRoom* RoomStorage::GetRandomBossRoom(char exit, vector<char> noExits) {
	bool found = false;
	DungeonRoom* result = nullptr;
	while (!found) {
		int minNum = 0;
		int maxNum = BossRooms.size() - 1;
		srand((unsigned int)time(NULL));
		int randomRoom = rand() % (maxNum - minNum + 1) + minNum;
		result = BossRooms[randomRoom];
		if (exit == 'U' && result->hasExitUp()) found = true;
		else if (exit == 'D' && result->hasExitDown()) found = true;
		else if (exit == 'L' && result->hasExitLeft()) found = true;
		else if (exit == 'R' && result->hasExitRight()) found = true;
	}
	return new DungeonRoom{ *result };
}

void RoomStorage::readAllRoomFiles() 
{
	// Maybe create a JSON file with all the filenames?
	// Not necessary as long as unwanted files don't end up in the rooms directory

	fs::directory_iterator Start;
	fs::directory_iterator End;

	// Iterate through the EntranceRooms folder and create a DungeonRoom instance with each file
	Start = fs::directory_iterator{ R"(.\src\ProceduralPrototype\rooms\EntranceRooms)" };
	End = fs::directory_iterator{};
	for (auto Iter{ Start }; Iter != End; ++Iter) {
		//cout << Iter->path().string() << '\n';
		DungeonRoom* room = new DungeonRoom(Iter->path().string());
		EntranceRooms.push_back(room);
	}

	// Iterate through the RegularRooms folder and create a DungeonRoom instance with each file
	Start = fs::directory_iterator{R"(.\src\ProceduralPrototype\rooms\RegularRooms)"};
	End = fs::directory_iterator{};
	for (auto Iter{ Start }; Iter != End; ++Iter) {
		//cout << Iter->path().string() << '\n';
		DungeonRoom* room = new DungeonRoom(Iter->path().string());
		RegularRooms.push_back(room);
	}

	// Iterate through the BossRooms folder and create a DungeonRoom instance with each file
	Start = fs::directory_iterator{ R"(.\src\ProceduralPrototype\rooms\BossRooms)" };
	End = fs::directory_iterator{};
	for (auto Iter{ Start }; Iter != End; ++Iter) {
		//cout << Iter->path().string() << '\n';
		DungeonRoom* room = new DungeonRoom(Iter->path().string());
		BossRooms.push_back(room);
	}
}

void RoomStorage::printRoomData(DungeonRoom* room) {
	cout << "NAME:\t" << room->getName() << endl;
	cout << "SIZE:\t" << room->getWidth() << "x" << room->getHeight() << endl;
	cout << endl;
	room->printLayoutTiles();
	room->printLayoutSpawns();
	cout << endl;
}