#include "RoomStorage.h"
#include "filesystem"

#include "DungeonRoom.h"

namespace fs = std::filesystem;

RoomStorage::RoomStorage()
{
	readAllRoomFiles();
	for (auto i : EntranceRooms) {
		printRoomData(i);
	}
	for (auto i : RegularRooms) {
		printRoomData(i);
	}
	for (auto i : BossRooms) {
		printRoomData(i);
	}
}

RoomStorage::~RoomStorage()
{

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
		EntranceRooms.push_back(room);
	}

	// Iterate through the BossRooms folder and create a DungeonRoom instance with each file
	Start = fs::directory_iterator{ R"(.\src\ProceduralPrototype\rooms\BossRooms)" };
	End = fs::directory_iterator{};
	for (auto Iter{ Start }; Iter != End; ++Iter) {
		//cout << Iter->path().string() << '\n';
		DungeonRoom* room = new DungeonRoom(Iter->path().string());
		EntranceRooms.push_back(room);
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