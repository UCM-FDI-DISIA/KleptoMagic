
#include "RoomStorage.h"
#include "DungeonRoom.h"
#include "filesystem"
#include <random>

namespace fs = std::filesystem;

RoomStorage::RoomStorage()
{
	readAllRoomFiles();
#ifdef _DEBUG
	for (auto i : EntranceRooms) {
		printRoomData(i);
	}
	for (auto i : RegularRooms) {
		printRoomData(i);
	}
	for (auto i : BossRooms) {
		printRoomData(i);
	}
#endif
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
					if (j == 'U' && i->hasExitUp())		doorsFilledN++;
					else if (j == 'D' && i->hasExitDown())	doorsFilledN++;
					else if (j == 'L' && i->hasExitLeft()) doorsFilledN++;
					else if (j == 'R' && i->hasExitRight())	doorsFilledN++;
				}

				// Check if the amount of filled exits is equal or greater than the size of the exitsNeeded vector.
				// If not, it means the room didn't have enough exits in the right places, and is invalid.
				if (doorsFilledN >= exitsNeeded.size()) {
					
					// The room must now have at least one more exit than the amount of exits needed to fill, so that it doesn't
					// close up the path and leave no exits open to continue generating.
					if (roomExitsN > exitsNeeded.size()) {
						// Check if any of the exits of the room are blacklisted BUT aren't in the neededExits vector, 
						// and add to results if none are.
						bool hasNoWrongExits = true;
						
						vector<char> roomExits = i->getUnusedExits();
						for (auto k : roomExits) {
							bool inExitsNeeded = false;
							bool inExitsBlacklisted = false;
							for (auto l : exitsNeeded) {
								if (k == l) inExitsNeeded = true;
							}
							for (auto m : exitsBlacklisted) {
								if (k == m) inExitsBlacklisted = true;
							}
							if (inExitsBlacklisted && !inExitsNeeded) hasNoWrongExits = false;
						}

						if (hasNoWrongExits) results.push_back(i);
					}
				}
			}

			// NOTE: In the case the tests above didn't find any rooms that met the condition, it will reattempt as a safety net.
			// It is missing implementation to restart the floor generation process from scratch: if the algorithm above doesn't find
			// any results, it means that none of the rooms in the files will ever fill that space properly, so it must restart
			// in order to not get stuck.

		}
	}

	std::random_device rd; // obtain a random number from hardware
	std::mt19937 gen(rd()); // seed the generator
	std::uniform_int_distribution<> distr(0, results.size() - 1); // define the range

	int randomRoom = distr(gen);
	return new DungeonRoom{ *results[randomRoom] };
}

DungeonRoom* RoomStorage::GetRandomSpecialRoom(vector<char> exitsNeeded) {
	vector<DungeonRoom*> results;
	while (results.size() < 1) {

		for (auto i : SpecialRooms) {

			// If the number of doors the room has is equal or greater than exits needed, continue
			int roomExitsN = i->getAmountOfExits();
			if (roomExitsN >= exitsNeeded.size()) {

				// Count how many of the required doors have been filled
				// If the number of filled doors is equal or greater than exits needed, continue
				int doorsFilledN = 0;
				for (auto j : exitsNeeded) {
					if (j == 'U' && i->hasExitUp())		doorsFilledN++;
					else if (j == 'D' && i->hasExitDown())	doorsFilledN++;
					else if (j == 'L' && i->hasExitLeft()) doorsFilledN++;
					else if (j == 'R' && i->hasExitRight())	doorsFilledN++;
				}

				// Check if the amount of filled exits is equal than the size of the exitsNeeded vector as well as the total exits of the room.
				// If it is, add to results.
				if (doorsFilledN == exitsNeeded.size() && doorsFilledN == roomExitsN) {
					results.push_back(i);
				}
			}

			// NOTE: In the case the tests above didn't find any rooms that met the condition, it will reattempt as a safety net.
			// It is missing implementation to restart the floor generation process from scratch: if the algorithm above doesn't find
			// any results, it means that none of the rooms in the files will ever fill that space properly, so it must restart
			// in order to not get stuck.

			// NOTE 2: it won't get stuck here due to changes to the main generation loop lol

		}
	}

	std::random_device rd; // obtain a random number from hardware
	std::mt19937 gen(rd()); // seed the generator
	std::uniform_int_distribution<> distr(0, results.size() - 1); // define the range

	int randomRoom = distr(gen);
	return new DungeonRoom{ *results[randomRoom] };
}

DungeonRoom* RoomStorage::GetRandomBossRoom(vector<char> exitsNeeded) {
	vector<DungeonRoom*> results;
	while (results.size() < 1) {

		for (auto i : BossRooms) {

			// If the number of doors the room has is equal or greater than exits needed, continue
			int roomExitsN = i->getAmountOfExits();
			if (roomExitsN >= exitsNeeded.size()) {

				// Count how many of the required doors have been filled
				// If the number of filled doors is equal or greater than exits needed, continue
				int doorsFilledN = 0;
				for (auto j : exitsNeeded) {
					if (j == 'U' && i->hasExitUp())		doorsFilledN++;
					else if (j == 'D' && i->hasExitDown())	doorsFilledN++;
					else if (j == 'L' && i->hasExitLeft()) doorsFilledN++;
					else if (j == 'R' && i->hasExitRight())	doorsFilledN++;
				}

				// Check if the amount of filled exits is equal than the size of the exitsNeeded vector as well as the total exits of the room.
				// If it is, add to results.
				if (doorsFilledN == exitsNeeded.size() && doorsFilledN == roomExitsN) {
					results.push_back(i);
				}
			}

			// NOTE: In the case the tests above didn't find any rooms that met the condition, it will reattempt as a safety net.
			// It is missing implementation to restart the floor generation process from scratch: if the algorithm above doesn't find
			// any results, it means that none of the rooms in the files will ever fill that space properly, so it must restart
			// in order to not get stuck.

			// NOTE 2: it won't get stuck here due to changes to the main generation loop lol

		}
	}

	std::random_device rd; // obtain a random number from hardware
	std::mt19937 gen(rd()); // seed the generator
	std::uniform_int_distribution<> distr(0, results.size() - 1); // define the range

	int randomRoom = distr(gen);
	return new DungeonRoom{ *results[randomRoom] };
}

void RoomStorage::readAllRoomFiles() 
{
	// Maybe create a JSON file with all the filenames?
	// Not necessary as long as unwanted files don't end up in the rooms directory

	fs::directory_iterator Start;
	fs::directory_iterator End;
	// Iterate through the EntranceRooms folder and create a DungeonRoom instance with each file
	Start = fs::directory_iterator{ R"(.\resources\room_data\EntranceRooms)" };
	End = fs::directory_iterator{};
	for (auto Iter{ Start }; Iter != End; ++Iter) {
		DungeonRoom* room = new DungeonRoom(Iter->path().string(), roomType::ENTRANCE);
		EntranceRooms.push_back(room);
	}

	// Iterate through the RegularRooms folder and create a DungeonRoom instance with each file
	Start = fs::directory_iterator{ R"(.\resources\room_data\RegularRooms)" };
	End = fs::directory_iterator{};
	for (auto Iter{ Start }; Iter != End; ++Iter) {
		DungeonRoom* room = new DungeonRoom(Iter->path().string(), roomType::REGULAR);
		RegularRooms.push_back(room);
	}

	// Iterate through the SpecialRooms folder and create a DungeonRoom instance with each file
	Start = fs::directory_iterator{ R"(.\resources\room_data\SpecialRooms)" };
	End = fs::directory_iterator{};
	for (auto Iter{ Start }; Iter != End; ++Iter) {
		DungeonRoom* room = new DungeonRoom(Iter->path().string(), roomType::SPECIAL);
		SpecialRooms.push_back(room);
	}

	// Iterate through the BossRooms folder and create a DungeonRoom instance with each file
	Start = fs::directory_iterator{ R"(.\resources\room_data\BossRooms)" };
	End = fs::directory_iterator{};
	for (auto Iter{ Start }; Iter != End; ++Iter) {
		DungeonRoom* room = new DungeonRoom(Iter->path().string(), roomType::BOSS);
		BossRooms.push_back(room);
	}
}

#ifdef _DEBUG
void RoomStorage::printRoomData(DungeonRoom* room) {
	cout << "NAME:\t" << room->getName() << endl;
	cout << "SIZE:\t" << room->getWidth() << "x" << room->getHeight() << endl;
	cout << endl;
	room->printLayoutTiles();
	room->printLayoutSpawns();
	cout << endl;
}
#endif