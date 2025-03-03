#include "DungeonFloor.h"

DungeonFloor::DungeonFloor(RoomStorage* RoomStorage) : roomstorage(RoomStorage)
{
	GenerateFloor();
	//PrintFloorLayout();
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

	// Set variables to indicate the current room being looked into for easy reference
	int CurrentRoomX = centerX;
	int CurrentRoomY = centerY;

	// Set variable to indicate what exit the current room being looked into will choose to get out (U, D, L, R)
	char CurrentRoomExit;

	// Set number of rooms to generate before boss room
	int roomsToGenerate = 5;



	// STEP 2: Generate a fixed amount of rooms charting a path towards a boss room


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