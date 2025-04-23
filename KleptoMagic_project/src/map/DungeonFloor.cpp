#include "DungeonFloor.h"
#include "SDL.h"
#include "SDL_image.h"
#include <random>
#include <vector>
#include "../game/EnemyUtils.h"

using namespace std;

DungeonFloor::DungeonFloor(int minWidth, int minHeight, int maxWidth, int maxHeight, int numRooms, RoomStorage* storage, SDL_Renderer* rend) : roomstorage(storage), renderer(rend)
{
	GenerateFloor(minWidth, minHeight, maxWidth, maxHeight, numRooms); 
	enemyutils().setDungeonFloor(this);

#ifdef _DEBUG
	PrintFloorLayout_Simple();
	PrintFloorLayout_Detailed();
	cout << "GENERATED" << endl;
#endif

}

DungeonFloor::~DungeonFloor()
{

}

void DungeonFloor::GenerateFloor(int minWidth, int minHeight, int maxWidth, int maxHeight, int numRooms) {
	// STEP 1: Preparation

	// Randomly choose the height and width of the dungeon floor from given parameters
	std::random_device rd; // obtain a random number from hardware
	std::mt19937 gen(rd()); // seed the generator
	std::uniform_int_distribution<> distrW(minWidth, maxWidth);
	std::uniform_int_distribution<> distrH(minHeight, maxHeight);
	floor_width = distrW(gen);
	floor_height = distrH(gen);

#ifdef _DEBUG
	cout << "FLOOR SIZE: " << floor_width << " " << floor_height << endl << endl;
#endif

	// Instantiate the room matrix
	floorLayout = vector<vector<DungeonRoom*>>(floor_width, vector<DungeonRoom*>(floor_height, 0));

	// Choose one random starting room out of storage, then place it in the center of the room matrix 
	// (or close to the center if on even numbers for size)
	startX = (floor_width) / 2;
	startY = (floor_height) / 2; 
	floorLayout[startX][startY] = roomstorage->GetRandomEntranceRoom();

	// Set variables to indicate the current room being looked into for easy reference, as well as the coordinates of the next room being generated
	int CurrentRoomX = startX;
	int CurrentRoomY = startY;
	int TargetRoomX = CurrentRoomX;
	int TargetRoomY = CurrentRoomY;

#ifdef _DEBUG
	cout << "START ROOM: "
		<< CurrentRoomX << " "
		<< CurrentRoomY << " "
		<< floorLayout[CurrentRoomX][CurrentRoomY]->getName() << " "
		<< floorLayout[CurrentRoomX][CurrentRoomY]->getAmountOfExits() << endl;
#endif

	// Set variable to indicate what exit the current room being looked into will choose to link the next room onto (U, D, L, R)
	char CurrentRoomExit;

	// Save the number of attempts at generating the floor for displaying
	int attempts = 1;

	// STEP 2: Generate a fixed amount of regular rooms charting a random path towards a boss room

	bool hasReachedGoal = false;
	while (!hasReachedGoal) {
		for (int i = 0; i < numRooms; i++) {

#ifdef _DEBUG
			cout << "GENERATING ROOM #" << i << endl << endl;
#endif

			// Define the location of the next room based on the exits and location of the current one.
#ifdef _DEBUG
			cout << "CURRENT ROOM: "
				<< CurrentRoomX << " "
				<< CurrentRoomY << " "
				<< floorLayout[CurrentRoomX][CurrentRoomY]->getName() << " "
				<< floorLayout[CurrentRoomX][CurrentRoomY]->getAmountOfExits() << endl;
#endif

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
			else if (CurrentRoomExit == '-') {
				// In the extremely rare (thought nonexistant) case that a room has no more free exits, restart generation as it is an
				// impossible layout. This is a failsafe that will likely never trigger
				break;
			}

#ifdef _DEBUG
			cout << "CHOSEN ROOM EXIT: "
				<< CurrentRoomExit << endl;
			cout << "TARGET ROOM LOCATION: "
				<< TargetRoomX << " " << TargetRoomY << endl;
#endif

			// Find out what exits need to be filled at the location the new room will be on, based on adjacent rooms and what exits they have.
			vector<char> exitsToConnect = ExitsToFillForSpace(TargetRoomX, TargetRoomY);

#ifdef _DEBUG
			cout << "EXITS THAT NEED TO BE FILLED: ";
			for (auto i : exitsToConnect) {
				cout << i;
			}
			if (exitsToConnect.size() > 1) {
				cout << " | THIS ROOM NEEDS TO FILL MORE THAN ONE EXIT!!!!!" << endl;
			}
#endif

			// Define what exits the new room can't have, based on other rooms adjacent to the target location for the new room, 
			// or if it would exit out of the matrix (floor map) limits.
			// When looking for blacklisted exits, look in all four directions, as even the previous room should be accounted for 
			// in the blacklist (as a safety net)
			vector<char> blacklistedExits = CheckSpaceAroundRoom(TargetRoomX, TargetRoomY);

#ifdef _DEBUG
			cout << "OCCUPIED SPACE, NO NEW EXITS HERE: ";
			for (auto i : blacklistedExits) {
				cout << i;
			}
#endif

			// In the rare case that rooms loop into themselves and a space needs to be filled with all spaces around it already occupied,
			// no new exits can be created, therefore it must restart generation.
			// Since generation is already very fast and going one step back will still be likely to end up in another room with limited
			// space generating soon, we will just restart it completely rather than backtracking.

#ifdef _DEBUG
			if (blacklistedExits.size() >= 4) {
				cout << " | THIS ROOM IS ENCLOSED! RESTART GENERATION..." << endl;
				break;
			}
			cout << endl;
#endif


			// Choose a random regular room that meets the defined criteria and place it at the target location
			floorLayout[TargetRoomX][TargetRoomY] = roomstorage->GetRandomRegularRoom(exitsToConnect, blacklistedExits);

#ifdef _DEBUG
			cout << "NEW ROOM: "
				<< TargetRoomX << " "
				<< TargetRoomY << " "
				<< floorLayout[TargetRoomX][TargetRoomY]->getName() << " "
				<< floorLayout[TargetRoomX][TargetRoomY]->getAmountOfExits() << endl;
#endif

			// Link all exits of the new room that connect to other rooms, including the previous one
			LinkExitsAtPosition(TargetRoomX, TargetRoomY, exitsToConnect);

#ifdef _DEBUG
			cout << "DOOR LINKAGE: "
				<< floorLayout[CurrentRoomX][CurrentRoomY]->linkU << " "
				<< floorLayout[CurrentRoomX][CurrentRoomY]->linkD << " "
				<< floorLayout[CurrentRoomX][CurrentRoomY]->linkL << " "
				<< floorLayout[CurrentRoomX][CurrentRoomY]->linkR << " | "
				<< floorLayout[TargetRoomX][TargetRoomY]->linkU << " "
				<< floorLayout[TargetRoomX][TargetRoomY]->linkD << " "
				<< floorLayout[TargetRoomX][TargetRoomY]->linkL << " "
				<< floorLayout[TargetRoomX][TargetRoomY]->linkR << endl;
#endif

			// Update the values for the current room location for the next iteration
			CurrentRoomX = TargetRoomX;
			CurrentRoomY = TargetRoomY;

#ifdef _DEBUG
			PrintFloorLayout_Detailed();
#endif

			if (i + 1 == numRooms) {
				hasReachedGoal = true;
			}
		}

		if (!hasReachedGoal) {
			// Reset the floor for new generation
			for (int i = 0; i < floor_width; i++) {
				for (int j = 0; j < floor_height; j++) {
					floorLayout[i][j] = nullptr;
				}
			}
			floorLayout = vector<vector<DungeonRoom*>>(floor_width, vector<DungeonRoom*>(floor_height, 0));
			CurrentRoomX = startX;
			CurrentRoomY = startY;
			TargetRoomX = CurrentRoomX;
			TargetRoomY = CurrentRoomY;
			floorLayout[startX][startY] = roomstorage->GetRandomEntranceRoom();

			attempts++;
		}
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

#ifdef _DEBUG
	cout << "SPECIAL/BOSS ROOM LOCATIONS: ";
	for (auto i : locations) {
		cout << i.x << "|" << i.y << " ";
	}
	cout << endl;
#endif

	// Decide which of those locations will be the boss room.
	// For now the condition will only be the room that is furthest away from the start.
	// In theory, even if it ends up close to the start because of the layout looping a lot, the player
	// will still want to look at every other room to gather loot.
	// After selecting, remove the position from the list of possible positions so it doesn't get in the way
	// of generating special rooms next.
	roomPos bossPos = roomPos{ 0,0 };
	double currentDist = 0;
	double dist = 0;
	int bossPosIndex = 0;

	for (int i = 0; i < locations.size(); i++) {
		double distX = startX - locations[i].x;
		double distY = startY - locations[i].y;
		dist = sqrt(pow(distX, 2) + pow(distY, 2));
		
		if (dist >= currentDist) {
			currentDist = dist;
			bossPos = locations[i];
			bossPosIndex = i;
		}
	}
	locations.erase(locations.begin() + bossPosIndex);

#ifdef _DEBUG
	cout << "LOCATION OF THE BOSS ROOM: ";
	cout << bossPos.x << "|" << bossPos.y << " " << endl;
#endif

	bossX = bossPos.x;
	bossY = bossPos.y;

	// Place in the boss room
	// Find out what exits need to be filled at the location the new room will be on, based on adjacent rooms and what exits they have.
#ifdef _DEBUG
	cout << "NOW PLACING THE BOSS ROOM!" << endl;
#endif

	vector<char> exitsToConnect = ExitsToFillForSpace(bossPos.x, bossPos.y);

#ifdef _DEBUG
	cout << "EXITS THAT NEED TO BE FILLED: ";
	for (auto j : exitsToConnect) {
		cout << j;
	}
	cout << endl;
	if (exitsToConnect.size() > 1) {
		cout << "THIS ROOM NEEDS TO FILL MORE THAN ONE EXIT!!!!!" << endl;
	}
#endif

	floorLayout[bossPos.x][bossPos.y] = roomstorage->GetRandomBossRoom(exitsToConnect);
	LinkExitsAtPosition(bossPos.x, bossPos.y, exitsToConnect);

	// Based on the number of rooms to generate, there can only be up to a third of those rooms worth of special rooms.
	// Pick a random set out of the possible locations to match.
	int specialsN = numRooms / 3;

#ifdef _DEBUG
	cout << "MAXIMUM NUMBER OF SPECIAL ROOMS ALLOWED (numRooms / 3): " << specialsN << endl;
	cout << "HOW MANY SPACES WERE THERE?: " << locations.size() - 1 << endl;
#endif

	vector<bool> locationsPicked(locations.size(), false);
	vector<int> indexesResult;
	for (int i = 0; i < specialsN && indexesResult.size() < locations.size(); i++) {
		bool found = false;
		std::mt19937 genInd(rd()); // seed the generator
		while (!found) {
			std::uniform_int_distribution<> distrInd(0, locations.size() - 1); // define the range
			int rndIndex = distrInd(genInd); // get a new random index
			if (!locationsPicked[rndIndex]) {
				indexesResult.push_back(rndIndex);
				locationsPicked[rndIndex] = true;
				found = true;
			}
		}
	}

#ifdef _DEBUG
	cout << "POSITIONS THAT WILL BE FILLED WITH SPECIAL ROOMS: ";
	for (auto i : indexesResult) {
		cout << i << " ";
	}
	cout << endl;
#endif


	// Place in special rooms at the final locations based on what exits they need to fill.

#ifdef _DEBUG
	cout << "NOW PLACING SPECIAL ROOMS" << endl;
#endif

	for (auto i : indexesResult) {
		roomPos pos = locations[i];
		// Find out what exits need to be filled at the location the new room will be on, based on adjacent rooms and what exits they have.
		vector<char> exitsToConnect = ExitsToFillForSpace(pos.x, pos.y);
#ifdef _DEBUG
		cout << "EXITS THAT NEED TO BE FILLED: ";
		for (auto j : exitsToConnect) {
			cout << j;
		}
		cout << endl;
		if (exitsToConnect.size() > 1) {
			cout << "THIS ROOM NEEDS TO FILL MORE THAN ONE EXIT!!!!!" << endl;
		}
#endif

		floorLayout[pos.x][pos.y] = roomstorage->GetRandomSpecialRoom(exitsToConnect);
		LinkExitsAtPosition(pos.x, pos.y, exitsToConnect);
	}

	// STEP 5: Now that the floor layout is complete, generate the tilemaps for all the rooms 
	// (since they now have their link booleans set, it can decide what exits to treat as walls as they lead to nowhere)

	for (int i = 0; i < floor_width; i++) {
		for (int j = 0; j < floor_height; j++) {
			DungeonRoom* room = floorLayout[i][j];
			if (room != nullptr) {
				room->CreateTilemap();
			}
		}
	}

	// TO IMPLEMENT: Turn 1 of the special rooms into a store if there are more than 1 special rooms
	if (specialsN > 2) {
		// pick a random one of the special rooms, based on indexesResult. Then, delete that room's index from indexesResult.
	}

	// TO IMPLEMENT: Add random content inside of the remainder of the special rooms

	currentX = startX;
	currentY = startY;

#ifdef _DEBUG
	system("CLS");
	cout << "--------------------------------" << endl;
	cout << "DONE! took " << attempts << " attempts" << endl;

	cout << "Starting room at " << startX << "," << startY << endl;
	cout << "Boss room at " << bossX << "," << bossY << endl;

	cout << "--------------------------------" << endl;
#endif
}

void DungeonFloor::render() {
	floorLayout[currentX][currentY]->render(renderer);
}

void DungeonFloor::update() {
	floorLayout[currentX][currentY]->getTilemap()->update();
}

int DungeonFloor::checkCollisions(int x, int y) {
	return floorLayout[currentX][currentY]->getTilemap()->checkCollision(x, y);
}

char DungeonFloor::checkEnterExit(int x, int y) {
	return floorLayout[currentX][currentY]->getTilemap()->checkExit(x, y);
}

Vector2D DungeonFloor::enterRoom(char exit) {

	Vector2D posAfterEnter;

	switch (exit) {
	case 'U':
		// moving up
		currentX = currentX - 1;
		// load room objects method here
		posAfterEnter = floorLayout[currentX][currentY]->PositionAfterEntering('D');
		break;
	case 'D':
		// moving down
		currentX = currentX + 1;
		// load room objects method here
		posAfterEnter = floorLayout[currentX][currentY]->PositionAfterEntering('U');
		break;
	case 'L':
		// moving left
		currentY = currentY - 1;
		// load room objects method here
		posAfterEnter = floorLayout[currentX][currentY]->PositionAfterEntering('R');
		break;
	case 'R':
		// moving right
		currentY = currentY + 1;
		// load room objects method here
		posAfterEnter = floorLayout[currentX][currentY]->PositionAfterEntering('L');
		break;
	case ' ':
		// entrance room spawn
		currentX = startX;
		currentY = startY;
		// load room objects method here
		posAfterEnter = floorLayout[currentX][currentY]->PositionAfterEntering(' ');
		break;
	default:
		break;
	}

#ifdef _DEBUG
	PrintFloorLayout_Detailed();
	cout << endl;
	cout << "Entering room '" << floorLayout[currentX][currentY]->getName() << "' of type '" << floorLayout[currentX][currentY]->getType() << "'";
	cout << endl;
#endif

	return posAfterEnter;
}

void DungeonFloor::spawnEnemies() {
	floorLayout[currentX][currentY]->spawnEnemies();
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
}

void DungeonFloor::addPos(roomPos pos, vector<roomPos>& locations) {
	if (std::find(locations.begin(), locations.end(), pos) != locations.end()); //nothing
	else locations.push_back(pos);
}

#ifdef _DEBUG
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
				if (i == currentX && j == currentY) {
					render_matrix[sp_i][sp_j] = '&';
					render_matrix[sp_i + 2][sp_j] = '&';
					render_matrix[sp_i][sp_j + 2] = '&';
					render_matrix[sp_i + 2][sp_j + 2] = '&';
				}

				// up exit
				if (floorLayout[i][j]->hasExitUp()) {
					if (floorLayout[i][j]->linkU) {
						render_matrix[sp_i]	[sp_j + 1]	= '#';
					}
					else {
						render_matrix[sp_i]	[sp_j + 1]	= '+';
					}
				}
				// down exit
				if (floorLayout[i][j]->hasExitDown()) {
					if (floorLayout[i][j]->linkD) {
						render_matrix[sp_i + 2][sp_j + 1] = '#';
					}
					else {
						render_matrix[sp_i + 2][sp_j + 1] = '+';
					}
				}
				// left exit
				if (floorLayout[i][j]->hasExitLeft()) {
					if (floorLayout[i][j]->linkL) {
						render_matrix[sp_i + 1][sp_j] = '#';
					}
					else {
						render_matrix[sp_i + 1][sp_j] = '+';
					}
				}
				// right exit
				if (floorLayout[i][j]->hasExitRight()) {
					if (floorLayout[i][j]->linkR) {
						render_matrix[sp_i + 1][sp_j + 2] = '#';
					}
					else {
						render_matrix[sp_i + 1][sp_j + 2] = '+';
					}
				}
			}
			else render_matrix[sp_i + 1][sp_j + 1] = '.';
		}
	}

	for (int i = 0; i < render_width; i++) {
		for (int j = 0; j < render_height; j++) {
			cout << render_matrix[i][j];
		}
		cout << endl;
	}
}
#endif