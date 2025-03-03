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
	cout << centerX << " " << centerY << endl;
	floorLayout[centerX][centerY] = roomstorage->CloneRandomEntranceRoom();
	cout << floorLayout[centerX][centerY]->getName() << endl;

	// Set variables to indicate the current room being looked into for easy reference, as well as the direction the new room is being positioned towards
	int CurrentRoomX = centerX;
	int CurrentRoomY = centerY;

	// Set variable to indicate what exit the current room being looked into will choose to link the next room onto (U, D, L, R)
	char CurrentRoomExit = floorLayout[CurrentRoomX][CurrentRoomY]->getRandomUnusedExit();
	cout << CurrentRoomExit << endl;

	// Set number of rooms to generate before boss room
	int roomsToGenerate = 5;

	// STEP 2: Generate a fixed amount of rooms charting a path towards a boss room

	for (int i = 0; i < roomsToGenerate; i++) {
		DungeonRoom* newRoom = roomstorage->FindRegularRoomWithExit(CurrentRoomExit);
		cout << newRoom->getName() << " | " << CurrentRoomX << " " << CurrentRoomY << " | " << CurrentRoomExit << " " << newRoom->getAmountOfExits() << endl;
		if (CurrentRoomExit == 'U') {
			floorLayout[CurrentRoomX][CurrentRoomY]->linkU = true;
			CurrentRoomX--;
			newRoom->linkD = true;
			floorLayout[CurrentRoomX][CurrentRoomY] = newRoom;
		}
		else if (CurrentRoomExit == 'D') {
			floorLayout[CurrentRoomX][CurrentRoomY]->linkD = true;
			CurrentRoomX++;
			newRoom->linkU = true;
			floorLayout[CurrentRoomX][CurrentRoomY] = newRoom;
		}
		else if (CurrentRoomExit == 'L') {
			floorLayout[CurrentRoomX][CurrentRoomY]->linkL = true;
			CurrentRoomY--;
			newRoom->linkR = true;
			floorLayout[CurrentRoomX][CurrentRoomY] = newRoom;
		}
		else if (CurrentRoomExit == 'R') {
			floorLayout[CurrentRoomX][CurrentRoomY]->linkR = true;
			CurrentRoomY++;
			newRoom->linkL = true;
			floorLayout[CurrentRoomX][CurrentRoomY] = newRoom;
		}
		CurrentRoomExit = floorLayout[CurrentRoomX][CurrentRoomY]->getRandomUnusedExit();
	}
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