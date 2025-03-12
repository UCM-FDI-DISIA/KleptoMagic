
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
	
	std::random_device rd; // obtain a random number from hardware
	std::mt19937 gen(rd()); // seed the generator
	std::uniform_int_distribution<> distr(0, EntranceRooms.size() - 1); // define the range

	int randomRoom = distr(gen);
	return new DungeonRoom{ *EntranceRooms[randomRoom] };
}
DungeonRoom* RoomStorage::GetRandomRegularRoom(char exit, vector<char> noExits) {
	vector<DungeonRoom*> results;
	while (results.size() < 1) {
		for (auto i : RegularRooms) {
			bool hasCorrectEntrance = false;
			if (exit == 'U' && i->hasExitDown()) hasCorrectEntrance = true;
			else if (exit == 'D' && i->hasExitUp()) hasCorrectEntrance = true;
			else if (exit == 'L' && i->hasExitRight()) hasCorrectEntrance = true;
			else if (exit == 'R' && i->hasExitLeft()) hasCorrectEntrance = true;

			bool hasNoWrongExits = true;
			for (auto j : noExits) {
				if (j == 'U' && i->hasExitUp() && exit != 'D') hasNoWrongExits = false;
				else if (j == 'D' && i->hasExitDown() && exit != 'U') hasNoWrongExits = false;
				else if (j == 'L' && i->hasExitLeft() && exit != 'R') hasNoWrongExits = false;
				else if (j == 'R' && i->hasExitRight() && exit != 'L') hasNoWrongExits = false;
			}
			
			/*
			cout << "+" << endl;
			cout << exit << " ";
			for (auto i : noExits) {
				cout << i;
			}
			cout << " " << hasCorrectEntrance << " " << hasNoWrongExits << " " << endl;
			cout << "+" << endl;
			*/

			if (hasCorrectEntrance && hasNoWrongExits) {
				results.push_back(i);
			}
		}
	}

	std::random_device rd; // obtain a random number from hardware
	std::mt19937 gen(rd()); // seed the generator
	std::uniform_int_distribution<> distr(0, results.size() - 1); // define the range

	int randomRoom = distr(gen);
	return new DungeonRoom{ *results[randomRoom] };
}

DungeonRoom* RoomStorage::GetRandomBossRoom(char exit, vector<char> noExits) {
	bool found = false;
	DungeonRoom* result = nullptr;
	while (!found) {
		std::random_device rd; // obtain a random number from hardware
		std::mt19937 gen(rd()); // seed the generator
		std::uniform_int_distribution<> distr(0, BossRooms.size() - 1); // define the range

		int randomRoom = distr(gen);
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