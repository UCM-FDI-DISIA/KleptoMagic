#include "DungeonFloor.h"

DungeonFloor::DungeonFloor(RoomStorage* RoomStorage) : roomstorage(RoomStorage)
{
	GenerateFloor();
	PrintFloorLayout();
}

DungeonFloor::~DungeonFloor()
{

}

void DungeonFloor::render() const {
	
}

void DungeonFloor::update() {

}

void DungeonFloor::GenerateFloor() {
	// STEP 1: Preparation

	// Randomly choose the height and width of the dungeon floor (CURRENTLY FIXED)
	floor_width = 25;
	floor_height = 25;

	// Instantiate the room matrix
	floorLayout = vector<vector<DungeonRoom*>>(floor_height, vector<DungeonRoom*>(floor_width, 0));

	// Choose one random starting room out of storage, then place it in the center of the room matrix (or close to the center if on even numbers for size)
	int centerX = (floor_width + 1) / 2;
	int centerY = (floor_height + 1) / 2;
	floorLayout[centerX][centerY] = roomstorage->GetRandomEntranceRoom();

	// Set variables to indicate the current room being looked into for easy reference, as well as the coordinates of the next room being generated
	int CurrentRoomX = centerX;
	int CurrentRoomY = centerY;
	int TargetRoomX = CurrentRoomX;
	int TargetRoomY = CurrentRoomY;

	// Set variable to indicate what exit the current room being looked into will choose to link the next room onto (U, D, L, R)
	char CurrentRoomExit;

	// Set number of rooms to generate before boss room
	int roomsToGenerate = 5;

	// STEP 2: Generate a fixed amount of rooms charting a path towards a boss room

	for (int i = 0; i < roomsToGenerate; i++) {
		//cout << CurrentRoomX << " " << CurrentRoomY << " " << CurrentRoomExit << endl;
		//cout << floorLayout[CurrentRoomX][CurrentRoomY]->getName() << " " << floorLayout[CurrentRoomX][CurrentRoomY]->getAmountOfExits() << endl;

		// Define the location of the next room based on the exits of the current one.
		// Then, define what exits the new room can't have, based on other rooms adjacent to the target location for the new room, 
		// or if it would exit out of the matrix's limits.
		// When looking for blacklisted exits, look in all four directions, as even the previous room should be accounted for in the blacklist (as a safety net)

		cout << "CURRENT ROOM: "
			<< CurrentRoomX << " "
			<< CurrentRoomY << " "
			<< floorLayout[CurrentRoomX][CurrentRoomY]->getName() << " "
			<< floorLayout[CurrentRoomX][CurrentRoomY]->getAmountOfExits() << endl;

		CurrentRoomExit = floorLayout[CurrentRoomX][CurrentRoomY]->getRandomUnusedExit();
		if (CurrentRoomExit == 'U') {
			TargetRoomX = CurrentRoomX--;
		}
		else if (CurrentRoomExit == 'D') {
			TargetRoomX = CurrentRoomX++;
		}
		else if (CurrentRoomExit == 'L') {
			TargetRoomY = CurrentRoomY--;
		}
		else if (CurrentRoomExit == 'R') {
			TargetRoomY = CurrentRoomY++;
		}

		cout << "CURRENT ROOM EXIT: " 
			<< CurrentRoomExit << endl;


		vector<char> blacklistedExits = CheckForAdjacentRooms(TargetRoomX, TargetRoomY);

		cout << "BLACKLISTED EXITS: ";
		for (auto i : blacklistedExits) {
			cout << i;
		}
		cout << endl;

		//DungeonRoom* newRoom = roomstorage->GetRandomRegularRoom(CurrentRoomExit, blacklistedExits);

		/*
		if (CurrentRoomExit == 'U') {
			floorLayout[CurrentRoomX][CurrentRoomY]->linkU = true;
			newRoom->linkD = true;
		}
		else if (CurrentRoomExit == 'D') {
			floorLayout[CurrentRoomX][CurrentRoomY]->linkD = true;
			newRoom->linkU = true;
		}
		else if (CurrentRoomExit == 'L') {
			floorLayout[CurrentRoomX][CurrentRoomY]->linkL = true;
			newRoom->linkR = true;
		}
		else if (CurrentRoomExit == 'R') {
			floorLayout[CurrentRoomX][CurrentRoomY]->linkR = true;
			newRoom->linkL = true;
		}

		// Place the new room in the target location
		floorLayout[TargetRoomX][TargetRoomY] = newRoom;
		*/

		/*
		cout << CurrentRoomX << " " << CurrentRoomY << " " << CurrentRoomExit << endl;
		cout << newRoom->getName() << " " << newRoom->getAmountOfExits() << endl;
		cout << "--------------" << endl;
		*/
	}
}

vector<char> DungeonFloor::CheckForAdjacentRooms(int x, int y) {
	vector<char> adjacentRooms;
	if (floorLayout[x--][y] != nullptr) {
		// Above
		adjacentRooms.push_back('U');
	}
	if (floorLayout[x++][y] != nullptr) {
		// Below
		adjacentRooms.push_back('D');
	}
	if (floorLayout[x][y--] != nullptr) {
		// Left
		adjacentRooms.push_back('L');
	}
	if (floorLayout[x][y++] != nullptr) {
		// Right
		adjacentRooms.push_back('R');
	}
	return  adjacentRooms;
}

void DungeonFloor::PrintFloorLayout() {
	for (int i = 0; i < floor_width; i++) {
		for (int j = 0; j < floor_height; j++) {
			if (floorLayout[i][j] != nullptr) {
				cout << "R";
			}
			else {
				cout << "*";
			}
		}
		cout << endl;
	}
}