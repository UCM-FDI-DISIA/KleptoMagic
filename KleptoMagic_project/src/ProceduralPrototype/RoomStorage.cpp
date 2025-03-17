
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
DungeonRoom* RoomStorage::GetRandomRegularRoom(vector<char> exitsNeeded, vector<char> exitsBlacklisted) {
	vector<DungeonRoom*> results;
	while (results.size() < 1) {

		for (auto i : RegularRooms) {

			// If the number of doors the room has is equal or greater than exits needed, continue
			int roomExitsN = i->getAmountOfExits();
			if (roomExitsN >= exitsNeeded.size()) {

				// Count how many of the required doors have been filled
				// If the number of filled doors is equal or greater than exits needed, continue
				int doorsFilledN = 0;
				for (auto j : exitsNeeded) {
					if (j == 'U' && i->hasExitDown())		doorsFilledN++;
					else if (j == 'D' && i->hasExitUp())	doorsFilledN++;
					else if (j == 'L' && i->hasExitRight()) doorsFilledN++;
					else if (j == 'R' && i->hasExitLeft())	doorsFilledN++;
				}

				// Check if the amount of filled exits is equal or greater than the size of the exitsNeeded vector.
				// If not, it means the room didn't have enough exits in the right places, and is invalid.
				if (doorsFilledN >= exitsNeeded.size()) {
					// If the room has the exact amount of exits needed, continue.
									// If the room has a greater amount of exits than needed, check if any of the exits are blacklisted.
					if (roomExitsN == exitsNeeded.size()) {
						// The room has the exact amount and location of exits needed.
						results.push_back(i);
					}
					else if (roomExitsN > exitsNeeded.size()) {
						// The room has the needed exits, but has additional exits as well. 
						// Check if any of the exits of the room are blacklisted BUT aren't in the neededExits vector, 
						// and add to results if none are.
						bool hasNoWrongExits = true;
						for (auto j : exitsNeeded) {
							for (auto k : exitsBlacklisted) {
								if (k == 'U' && i->hasExitUp() && j != 'D') hasNoWrongExits = false;
								else if (k == 'D' && i->hasExitDown() && j != 'U') hasNoWrongExits = false;
								else if (k == 'L' && i->hasExitLeft() && j != 'R') hasNoWrongExits = false;
								else if (k == 'R' && i->hasExitRight() && j != 'L') hasNoWrongExits = false;
							}
						}
						if (hasNoWrongExits) results.push_back(i);
					}
				}
			}

			// NOTE: In the case the tests above didn't find any rooms that met the condition, it will reattempt as a safety net.
			// It is missing implementation to restart the generation process from scratch: if the algorithm above doesn't find
			// ANY results, it means that none of the rooms in the files will ever fill that space properly, so it must restart.

		}
		/*
		for (auto i : RegularRooms) {
			bool hasCorrectEntrance = false;
			bool hasNoWrongExits = true;
			for (auto j : exits) {
				if (j == 'U' && i->hasExitDown()) hasCorrectEntrance = true;
				else if (j == 'D' && i->hasExitUp()) hasCorrectEntrance = true;
				else if (j == 'L' && i->hasExitRight()) hasCorrectEntrance = true;
				else if (j == 'R' && i->hasExitLeft()) hasCorrectEntrance = true;

				for (auto k : noExits) {
					if (k == 'U' && i->hasExitUp() && j != 'D') hasNoWrongExits = false;
					else if (k == 'D' && i->hasExitDown() && j != 'U') hasNoWrongExits = false;
					else if (k == 'L' && i->hasExitLeft() && j != 'R') hasNoWrongExits = false;
					else if (k == 'R' && i->hasExitRight() && j != 'L') hasNoWrongExits = false;
				}
			}

			if (hasCorrectEntrance && hasNoWrongExits) {
				results.push_back(i);
			}
		}
		*/
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