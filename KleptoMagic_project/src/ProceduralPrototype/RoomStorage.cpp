
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

DungeonRoom* RoomStorage::CloneRandomEntranceRoom() {
	int minNum = 0;
	int maxNum = EntranceRooms.size() - 1;
	srand((unsigned int)time(NULL));
	int randomRoom = rand() % (maxNum - minNum + 1) + minNum;
	return new DungeonRoom{ *EntranceRooms[randomRoom] };
}

DungeonRoom* RoomStorage::CloneRandomRegularRoom() {
	int minNum = 0;
	int maxNum = RegularRooms.size() - 1;
	srand((unsigned int)time(NULL));
	int randomRoom = rand() % (maxNum - minNum + 1) + minNum;
	return new DungeonRoom{ *RegularRooms[randomRoom] };
}

DungeonRoom* RoomStorage::CloneRandomBossRoom() {
	int minNum = 0;
	int maxNum = BossRooms.size() - 1;
	srand((unsigned int)time(NULL));
	int randomRoom = rand() % (maxNum - minNum + 1) + minNum;
	return new DungeonRoom{ *BossRooms[randomRoom] };
}

DungeonRoom* RoomStorage::FindRegularRoomWithExit(char exit) {
	bool found = false;
	DungeonRoom* result = nullptr;
	int minNum = 0;
	int maxNum = RegularRooms.size() - 1;
	srand((unsigned int)time(NULL));
	while (!found) {
		int randomRoom = rand() % (maxNum - minNum + 1) + minNum;
		result = RegularRooms[randomRoom];
		if (exit == 'U' && result->hasExitUp()) found = true;
		else if (exit == 'D' && result->hasExitDown()) found = true;
		else if (exit == 'L' && result->hasExitLeft()) found = true;
		else if (exit == 'R' && result->hasExitRight()) found = true;
	}
	return new DungeonRoom{ *result };
}

DungeonRoom* RoomStorage::FindBossRoomWithExit(char exit) {
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