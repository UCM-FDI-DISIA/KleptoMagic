#include "DungeonFloor.h"
#include <random>
#include <vector>

DungeonFloor::DungeonFloor(RoomStorage* RoomStorage) : roomstorage(RoomStorage)
{
	GenerateFloor();
	PrintFloorLayout_Detailed();
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

	// Randomly choose the height and width of the dungeon floor

	int minWidth = 10;
	int minHeight = 10;
	int maxWidth = 20;
	int maxHeight = 20;

	std::random_device rd; // obtain a random number from hardware
	std::mt19937 gen(rd()); // seed the generator
	std::uniform_int_distribution<> distrW(minWidth, maxWidth);
	std::uniform_int_distribution<> distrH(minHeight, maxHeight);

	floor_width = distrW(gen);
	floor_height = distrH(gen);

	cout << "FLOOR SIZE: " << floor_width << " " << floor_height << endl << endl;

	// Set number of rooms to generate before boss room
	int roomsToGenerate = 10;

	// Instantiate the room matrix
	floorLayout = vector<vector<DungeonRoom*>>(floor_width, vector<DungeonRoom*>(floor_height, 0));

	// Choose one random starting room out of storage, then place it in the center of the room matrix (or close to the center if on even numbers for size)
	int centerX = (floor_width) / 2;
	int centerY = (floor_height) / 2;
	floorLayout[centerX][centerY] = roomstorage->GetRandomEntranceRoom();

	// Set variables to indicate the current room being looked into for easy reference, as well as the coordinates of the next room being generated
	int CurrentRoomX = centerX;
	int CurrentRoomY = centerY;
	int TargetRoomX = CurrentRoomX;
	int TargetRoomY = CurrentRoomY;

	cout << "START ROOM: "
		<< CurrentRoomX << " "
		<< CurrentRoomY << " "
		<< floorLayout[CurrentRoomX][CurrentRoomY]->getName() << " "
		<< floorLayout[CurrentRoomX][CurrentRoomY]->getAmountOfExits() << endl;

	// Set variable to indicate what exit the current room being looked into will choose to link the next room onto (U, D, L, R)
	char CurrentRoomExit = 'N';

	PrintFloorLayout_Simple();

	// STEP 2: Generate a fixed amount of regular rooms charting a random path towards a boss room

	for (int i = 0; i < roomsToGenerate; i++) {

		cout << "GENERATING ROOM #" << i << endl << endl;

		// Define the location of the next room based on the exits and location of the current one.
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

		cout << "CHOSEN ROOM EXIT: " 
			<< CurrentRoomExit << endl;
		cout << "TARGET ROOM LOCATION: "
			<< TargetRoomX << " " << TargetRoomY << endl;

		// Find out what exits need to be filled at the location the new room will be on, based on adjacent rooms and what exits they have.
		vector<char> exitsToConnect = ExitsToFillForSpace(TargetRoomX, TargetRoomY);
		cout << "EXITS THAT NEED TO BE FILLED: ";
		for (auto i : exitsToConnect) {
			cout << i;
		}
		cout << endl;
		if (exitsToConnect.size() > 1) {
			cout << "THIS ROOM NEEDS TO FILL MORE THAN ONE EXIT!!!!!" << endl;
		}

		// Define what exits the new room can't have, based on other rooms adjacent to the target location for the new room, 
		// or if it would exit out of the matrix (floor map) limits.
		// When looking for blacklisted exits, look in all four directions, as even the previous room should be accounted for 
		// in the blacklist (as a safety net)
		vector<char> blacklistedExits = CheckSpaceAroundRoom(TargetRoomX, TargetRoomY);
		cout << "OCCUPIED SPACE, NO NEW EXITS HERE: ";
		for (auto i : blacklistedExits) {
			cout << i;
		}
		cout << endl;

		// Choose a random regular room that meets the defined criteria and place it at the target location
		floorLayout[TargetRoomX][TargetRoomY] = roomstorage->GetRandomRegularRoom(exitsToConnect, blacklistedExits);;

		cout << "NEW ROOM: "
			<< TargetRoomX << " "
			<< TargetRoomY << " "
			<< floorLayout[TargetRoomX][TargetRoomY]->getName() << " "
			<< floorLayout[TargetRoomX][TargetRoomY]->getAmountOfExits() << endl;

		// Link the exit of the previous room up with the new room
		// UNNECESSARY: code below links all exits now
		/*
		if (CurrentRoomExit == 'U') {
			floorLayout[CurrentRoomX][CurrentRoomY]->linkU = true;
			floorLayout[TargetRoomX][TargetRoomY]->linkD = true;
		}
		else if (CurrentRoomExit == 'D') {
			floorLayout[CurrentRoomX][CurrentRoomY]->linkD = true;
			floorLayout[TargetRoomX][TargetRoomY]->linkU = true;
		}
		else if (CurrentRoomExit == 'L') {
			floorLayout[CurrentRoomX][CurrentRoomY]->linkL = true;
			floorLayout[TargetRoomX][TargetRoomY]->linkR = true;
		}
		else if (CurrentRoomExit == 'R') {
			floorLayout[CurrentRoomX][CurrentRoomY]->linkR = true;
			floorLayout[TargetRoomX][TargetRoomY]->linkL = true;
		}
		*/

		// Link all exits of the new room that connect to other rooms, including the previous one
		int x = TargetRoomX;
		int y = TargetRoomY;
		vector<char> exits = exitsToConnect;
		DungeonRoom* currentRoom = nullptr;
		for (auto i : exits) {
			// Room above
			if (i == 'U') {
				currentRoom = floorLayout[x - 1][y];
				if (floorLayout[x - 1][y] != nullptr) {
					floorLayout[x - 1][y]->linkD = true;
					floorLayout[x][y]->linkU = true;
				}
			}
			// Room below
			if (i == 'D') {
				currentRoom = floorLayout[x + 1][y];
				if (floorLayout[x + 1][y] != nullptr) {
					floorLayout[x + 1][y]->linkU = true;
					floorLayout[x][y]->linkD = true;
				}
			}
			// Room left
			if (i == 'L') {
				currentRoom = floorLayout[x][y - 1];
				if (floorLayout[x][y - 1] != nullptr) {
					floorLayout[x][y - 1]->linkR = true;
					floorLayout[x][y]->linkL = true;
				}
			}
			// Room right
			if (i == 'R') {
				currentRoom = floorLayout[x][y + 1];
				if (floorLayout[x][y + 1] != nullptr) {
					floorLayout[x][y + 1]->linkL = true;
					floorLayout[x][y]->linkR = true;
				}
			}
		}

		cout << "DOOR LINKAGE: "
			<< floorLayout[CurrentRoomX][CurrentRoomY]->linkU << " "
			<< floorLayout[CurrentRoomX][CurrentRoomY]->linkD << " "
			<< floorLayout[CurrentRoomX][CurrentRoomY]->linkL << " "
			<< floorLayout[CurrentRoomX][CurrentRoomY]->linkR << " | "
			<< floorLayout[TargetRoomX][TargetRoomY]->linkU << " "
			<< floorLayout[TargetRoomX][TargetRoomY]->linkD << " "
			<< floorLayout[TargetRoomX][TargetRoomY]->linkL << " "
			<< floorLayout[TargetRoomX][TargetRoomY]->linkR << endl;

		// Update the values for the current room location for the next iteration
		CurrentRoomX = TargetRoomX;
		CurrentRoomY = TargetRoomY;

		PrintFloorLayout_Detailed();
	}

	// STEP 3: Locate all possible positions at which special rooms and the boss room need can be placed.
	// These are all the locations that exits that remain empty after generation lead to.

	vector<roomPos> locations;

	for (int i = 0; i < floor_width; i++) {
		for (int j = 0; j < floor_height; j++) {
			DungeonRoom* room = floorLayout[i][j];
			if (room != nullptr) {
				
				int numExits = room->getAmountOfExits();

				if (room->hasExitUp() && !room->linkU) {
					roomPos newPos;
					newPos.x = i - 1;
					newPos.y = j;
					addPos(newPos, locations);
				}
				if (room->hasExitDown() && !room->linkD) {
					roomPos newPos;
					newPos.x = i + 1;
					newPos.y = j;
					addPos(newPos, locations);
				}
				if (room->hasExitLeft() && !room->linkL) {
					roomPos newPos;
					newPos.x = i;
					newPos.y = j - 1;
					addPos(newPos, locations);
				}
				if (room->hasExitRight() && !room->linkR) {
					roomPos newPos;
					newPos.x = i;
					newPos.y = j + 1;
					addPos(newPos, locations);
				}
			}
		}
	}

	cout << "SPECIAL/BOSS ROOM LOCATIONS: ";
	for (auto i : locations) {
		cout << i.x << "|" << i.y << " ";
	}
	cout << endl;

	// Place in rooms at the locations based on what exits they need to fill.
	for (auto i : locations);

	/*
	for (auto i : locations) {
		cout << i.x << "|" << i.y << endl;
		floorLayout[i.x][i.y] = new DungeonRoom(R"(.\src\ProceduralPrototype\rooms\SpecialRooms\0000_3_3_test.txt")", roomType::SPECIAL);
		PrintFloorLayout_Detailed();
	}
	*/



}

vector<char> DungeonFloor::CheckSpaceAroundRoom(int x, int y) {
	vector<char> invalidExits;
	auto currRoom = floorLayout[x][y];

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

vector<char> DungeonFloor::ExitsToFillForSpace(int x, int y) {
	vector<char> results;
	DungeonRoom* currentRoom = nullptr;

	// Above
	if (x - 1 >= 0) {
		currentRoom = floorLayout[x - 1][y];
		if (currentRoom != nullptr && currentRoom->hasExitDown()) {
			results.push_back('U');
		}
	}

	// Below
	if (x + 1 < floor_width) {
		currentRoom = floorLayout[x + 1][y];
		if (currentRoom != nullptr && currentRoom->hasExitUp()) {
			results.push_back('D');
		}
	}

	// Left
	if (y - 1 >= 0) {
		currentRoom = floorLayout[x][y - 1];
		if (currentRoom != nullptr && currentRoom->hasExitRight()) {
			results.push_back('L');
		}
	}

	// Right
	if (y + 1 < floor_height) {
		currentRoom = floorLayout[x][y + 1];
		if (currentRoom != nullptr && currentRoom->hasExitLeft()) {
			results.push_back('R');
		}
	}

	return results;
}

void DungeonFloor::LinkExitsAtPosition(int x, int y, vector<char> exits) {
	DungeonRoom* currentRoom = nullptr;

	for (auto i : exits) {
		// Room above
		if (i == 'U') {
			currentRoom = floorLayout[x - 1][y];
			if (currentRoom != nullptr) {
				currentRoom->linkD = true;
			}
		}
		// Room below
		if (i == 'D') {
			currentRoom = floorLayout[x + 1][y];
			if (currentRoom != nullptr) {
				currentRoom->linkU = true;
			}
		}
		// Room left
		if (i == 'L') {
			currentRoom = floorLayout[x][y - 1];
			if (currentRoom != nullptr) {
				currentRoom->linkR = true;
			}
		}
		// Room right
		if (i == 'R') {
			currentRoom = floorLayout[x][y + 1];
			if (currentRoom != nullptr) {
				currentRoom->linkL = true;
			}
		}
		cout << "test" << endl;
	}
	/*
	for (auto i : exits) {
		// Above
		if (i == 'U' && x - 1 >= 0) {
			if (floorLayout[x - 1][y] != nullptr) {
				floorLayout[x - 1][y]->linkD = true;
			}
		}
		// Below
		else if (i == 'D' && x + 1 < floor_width) {
			if (floorLayout[x + 1][y] != nullptr) {
				floorLayout[x + 1][y]->linkU = true;
			}
		}
		// Left
		else if (i == 'L' && y - 1 >= 0) {
			if (floorLayout[x][y - 1] != nullptr) {
				floorLayout[x][y - 1]->linkR = true;
			}
		}
		// Right
		else if (i == 'R' && y + 1 < floor_height) {
			if (floorLayout[x][y + 1] != nullptr) {
				floorLayout[x][y + 1]->linkL = true;
			}
		}
	}
	*/
}

void DungeonFloor::addPos(roomPos pos, vector<roomPos>& locations) {
	if (std::find(locations.begin(), locations.end(), pos) != locations.end()); //nothing
	else locations.push_back(pos);
}

void DungeonFloor::PrintFloorLayout_Simple() {
	for (int i = 0; i < floor_width; i++) {
		for (int j = 0; j < floor_height; j++) {
			if (floorLayout[i][j] != nullptr) {
				if (floorLayout[i][j]->getType() == roomType::ENTRANCE) cout << "E";
				else if (floorLayout[i][j]->getType() == roomType::REGULAR) cout << "R";
				else if (floorLayout[i][j]->getType() == roomType::SPECIAL) cout << "S";
				else if (floorLayout[i][j]->getType() == roomType::BOSS) cout << "B";
			}
			else cout << "+";
		}
		cout << endl;
	}
	cout << endl << endl;
}

void DungeonFloor::PrintFloorLayout_Detailed() {

	int render_width = floor_width * 3;
	int render_height = floor_height * 3;

	vector<vector<char>> render_matrix = vector<vector<char>>(render_width, vector<char>(render_height, ' '));

	for (int i = 0; i < floor_width; i++) {
		for (int j = 0; j < floor_height; j++) {

			int sp_i = i * 3;
			int sp_j = j * 3;

			if (floorLayout[i][j] != nullptr) {
				// center of room

				if (floorLayout[i][j]->getType() == roomType::ENTRANCE)		render_matrix[sp_i + 1][sp_j + 1] = 'E';
				else if (floorLayout[i][j]->getType() == roomType::REGULAR) render_matrix[sp_i + 1][sp_j + 1] = 'R';
				else if (floorLayout[i][j]->getType() == roomType::SPECIAL) render_matrix[sp_i + 1][sp_j + 1] = 'S';
				else if (floorLayout[i][j]->getType() == roomType::BOSS)	render_matrix[sp_i + 1][sp_j + 1] = 'B';

				// corners of room
				/*
				render_matrix[sp_i]		[sp_j]		= '+';
				render_matrix[sp_i + 2]	[sp_j]		= '+';
				render_matrix[sp_i]		[sp_j + 2]	= '+';
				render_matrix[sp_i + 2]	[sp_j + 2]	= '+';
				*/

				// up exit
				if (floorLayout[i][j]->hasExitUp()) {
					if (floorLayout[i][j]->linkU) {
						render_matrix[sp_i]	[sp_j + 1]	= 'L';
					}
					else {
						render_matrix[sp_i]	[sp_j + 1]	= 'D';
					}
				}
				// down exit
				if (floorLayout[i][j]->hasExitDown()) {
					if (floorLayout[i][j]->linkD) {
						render_matrix[sp_i + 2][sp_j + 1] = 'L';
					}
					else {
						render_matrix[sp_i + 2][sp_j + 1] = 'D';
					}
				}
				// left exit
				if (floorLayout[i][j]->hasExitLeft()) {
					if (floorLayout[i][j]->linkL) {
						render_matrix[sp_i + 1][sp_j] = 'L';
					}
					else {
						render_matrix[sp_i + 1][sp_j] = 'D';
					}
				}
				// right exit
				if (floorLayout[i][j]->hasExitRight()) {
					if (floorLayout[i][j]->linkR) {
						render_matrix[sp_i + 1][sp_j + 2] = 'L';
					}
					else {
						render_matrix[sp_i + 1][sp_j + 2] = 'D';
					}
				}
			}
			else render_matrix[sp_i + 1][sp_j + 1] = '-';
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