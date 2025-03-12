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
	floor_width = 10;
	floor_height = 10;

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
	char CurrentRoomExit = 'N';

	// Set number of rooms to generate before boss room
	int roomsToGenerate = 1;

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
			TargetRoomX = CurrentRoomX - 1;
		}
		else if (CurrentRoomExit == 'D') {
			TargetRoomX = CurrentRoomX + 1;
		}
		else if (CurrentRoomExit == 'L') {
			TargetRoomY = CurrentRoomY - 1;
		}
		else if (CurrentRoomExit == 'R') {
			TargetRoomY = CurrentRoomY + 1;
		}

		cout << "CURRENT ROOM EXIT: " 
			<< CurrentRoomExit << endl;

		vector<char> blacklistedExits = CheckForInvalidExits(TargetRoomX, TargetRoomY);

		cout << "BLACKLISTED EXITS: ";
		for (auto i : blacklistedExits) {
			cout << i;
		}
		cout << endl;

		DungeonRoom* newRoom = roomstorage->GetRandomRegularRoom(CurrentRoomExit, blacklistedExits);
		floorLayout[TargetRoomX][TargetRoomY] = newRoom;

		cout << "NEW ROOM: " 
			<< TargetRoomX << " "
			<< TargetRoomY << " "
			<< floorLayout[TargetRoomX][TargetRoomY]->getName() << " "
			<< newRoom->getName() << " "
			<< floorLayout[TargetRoomX][TargetRoomY]->getAmountOfExits() << " " 
			<< newRoom->getAmountOfExits() << endl;

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
		*/

		cout << "DOOR LINKAGE: "
			<< floorLayout[CurrentRoomX][CurrentRoomY]->linkU << " "
			<< floorLayout[CurrentRoomX][CurrentRoomY]->linkD << " "
			<< floorLayout[CurrentRoomX][CurrentRoomY]->linkL << " "
			<< floorLayout[CurrentRoomX][CurrentRoomY]->linkR << " | "
			<< newRoom->linkU << " "
			<< newRoom->linkD << " "
			<< newRoom->linkL << " "
			<< newRoom->linkR << endl;

		CurrentRoomX = TargetRoomX;
		CurrentRoomY = TargetRoomY;

		cout << "----------" << endl;

		/*
		cout << CurrentRoomX << " " << CurrentRoomY << " " << CurrentRoomExit << endl;
		cout << newRoom->getName() << " " << newRoom->getAmountOfExits() << endl;
		cout << "--------------" << endl;
		*/
	}
}

vector<char> DungeonFloor::CheckForInvalidExits(int x, int y) {
	vector<char> invalidExits;

	// Above
	if (x - 1 >= 0) {
		auto roomU = floorLayout[x - 1][y];
		if (roomU != nullptr) {
			invalidExits.push_back('U');
		}
	}
	else {
		invalidExits.push_back('U');
	}

	// Below
	if (x + 1 < floor_width) {
		auto roomD = floorLayout[x + 1][y];
		if (roomD != nullptr) {
			invalidExits.push_back('D');
		}
	}
	else {
		invalidExits.push_back('D');
	}

	// Left
	if (y - 1 >= 0) {
		auto roomL = floorLayout[x][y - 1];
		if (roomL != nullptr) {
			invalidExits.push_back('L');
		}
	}
	else {
		invalidExits.push_back('L');
	}

	// Right
	if (y + 1 < floor_height) {
		auto roomR = floorLayout[x][y + 1];
		if (roomR != nullptr) {
			invalidExits.push_back('R');
		}
	}
	else {
		invalidExits.push_back('R');
	}

	return  invalidExits;
}

void DungeonFloor::PrintFloorLayout() {

	int render_width = floor_width * 3;
	int render_height = floor_height * 3;

	vector<vector<char>> render_matrix = vector<vector<char>>(render_height, vector<char>(render_width, ' '));

	for (int i = 0; i < floor_width; i++) {
		for (int j = 0; j < floor_height; j++) {

			int sp_x = i * 3;
			int sp_y = j * 3;

			/*
				render_matrix[sp_x]		[sp_y] = '*';
				render_matrix[sp_x + 1]	[sp_y] = '*';
				render_matrix[sp_x + 2]	[sp_y] = '*';

				render_matrix[sp_x]		[sp_y + 1] = '*';
				render_matrix[sp_x + 1]	[sp_y + 1] = 'R';
				render_matrix[sp_x + 2]	[sp_y + 1] = '*';

				render_matrix[sp_x]		[sp_y + 2] = '*';
				render_matrix[sp_x + 1]	[sp_y + 2] = '*';
				render_matrix[sp_x + 2]	[sp_y + 2] = '*';
			*/

			if (floorLayout[i][j] != nullptr) {
				// center of room
				render_matrix[sp_x + 1]	[sp_y + 1]	= 'R';

				// corners of room
				render_matrix[sp_x]		[sp_y]		= '+';
				render_matrix[sp_x + 2]	[sp_y]		= '+';
				render_matrix[sp_x]		[sp_y + 2]	= '+';
				render_matrix[sp_x + 2]	[sp_y + 2]	= '+';

				// up exit
				if (floorLayout[i][j]->hasExitUp()) {
					if (floorLayout[i][j]->linkU) {
						render_matrix[sp_x + 1][sp_y] = 'L';
					}
					else {
						render_matrix[sp_x + 1][sp_y] = 'D';
					}
				}
				// down exit
				if (floorLayout[i][j]->hasExitDown()) {
					if (floorLayout[i][j]->linkD) {
						render_matrix[sp_x + 1][sp_y + 2] = 'L';
					}
					else {
						render_matrix[sp_x + 1][sp_y + 2] = 'D';
					}
				}
				// left exit
				if (floorLayout[i][j]->hasExitLeft()) {
					if (floorLayout[i][j]->linkL) {
						render_matrix[sp_x][sp_y + 1] = 'L';
					}
					else {
						render_matrix[sp_x][sp_y + 1] = 'D';
					}
				}
				// right exit
				if (floorLayout[i][j]->hasExitRight()) {
					if (floorLayout[i][j]->linkR) {
						render_matrix[sp_x + 2][sp_y + 1] = 'L';
					}
					else {
						render_matrix[sp_x + 2][sp_y + 1] = 'D';
					}
				}
			}
			else {
				// no room
				render_matrix[sp_x + 1]	[sp_y + 1] = '-';
			}

		}
	}

	for (int i = 0; i < render_width; i++) {
		for (int j = 0; j < render_height; j++) {
			cout << render_matrix[i][j];
		}
		cout << endl;
	}
	cout << endl;
}