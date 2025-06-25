#include "DungeonRoom.h"
#include "SDL.h"
#include "SDL_render.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <random>

#include "./../utils/StringUtils.h"
using namespace std;

DungeonRoom::DungeonRoom(string filename, roomType type) : room_type(type), opened(false)
{
#ifdef _DEBUG
	cout << endl;
	cout << "CREATING ROOM: " << filename << endl;
#endif

	// Split every element of the filename in order to isolate important data built onto it (room size, available exits)
	vector<string> filename_string_separated = string_splitStringByDelimiter(filename, "\\");
	string actual_filename = filename_string_separated[filename_string_separated.size() - 1];
	vector<string> room_data_from_filename = string_splitStringByDelimiter(actual_filename, "_");
	doorU = room_data_from_filename[0][0] == '1';
	doorD = room_data_from_filename[0][1] == '1';
	doorL = room_data_from_filename[0][2] == '1';
	doorR = room_data_from_filename[0][3] == '1';
	room_width = stoi(room_data_from_filename[1]);
	room_height = stoi(room_data_from_filename[2]);
	room_name = string_splitStringByDelimiter(room_data_from_filename[3], ".")[0];

	ifstream roomFile;

	// ROOM TILE FORMAT
	// W -> wall
	// B -> blank (space outside walls, pitch black)
	// V -> void/hole
	// * -> walkable floor
	// U, D, L, R -> doors corresponding to up, down, left, right exits

	roomFile.open(filename);

	// Reading the first matrix in the file, AKA the tile layout
	roomTiles = vector<vector<char>>(room_width, vector<char>(room_height, 0));
	string line;
	int row = 0;
	while (getline(roomFile, line) && row < room_height) {
		stringstream ss(line);
		for (int i = 0; i < room_width; i++) {
			roomTiles[i][row] = line[i];
		}
		row++;
	}

	//getline(roomFile, line); // Ignoring the blank line between both matrices in the file
	row = 0;

	//Reading the second matrix in the file, AKA the spawn layout
	while (getline(roomFile, line) && row < room_height) {
		stringstream ss(line);
		for (int i = 0; i < room_width; i++) {
			EnemyNames name;
			switch (line[i]) {
			case 's':
				// slime
				name = ENEMY_SLIME;
				roomEnemies.push_back(spawnData_enemy(Vector2D{ (float)i, (float)row }, name));
				break;
			case 'a':
				// archer
				name = ENEMY_ARCHER;
				roomEnemies.push_back(spawnData_enemy(Vector2D{ (float)i, (float)row }, name));
				break;
			case 'l':
				// armor
				name = ENEMY_ARMOR;
				roomEnemies.push_back(spawnData_enemy(Vector2D{ (float)i, (float)row }, name));
				break;
			case 'g':
				name = ENEMY_GHOST;
				roomEnemies.push_back(spawnData_enemy(Vector2D{ (float)i, (float)row }, name));
				break;
			case 'b':
				// boss
				name = ENEMY_BOSS;
				roomEnemies.push_back(spawnData_enemy(Vector2D{ (float)i, (float)row }, name));
				break;

			case 'n':
				name = ENEMY_NECRO;
				roomEnemies.push_back(spawnData_enemy(Vector2D{ (float)i, (float)row }, name));
				break;
			case 'u':
				name = ENEMY_UPGRADE;
				roomEnemies.push_back(spawnData_enemy(Vector2D{ (float)i, (float)row }, name));
				break;
			}
		}
		row++;
	}

	// If the room has no enemies, it is cleared by default and does not need to be opened
	if (roomEnemies.size() <= 0) cleared = true;
	else cleared = false;

	// Find and store the center and exit locations
	CenterX = room_width / 2;
	CenterY = room_height / 2;
	for (int i = 0; i < room_height; i++) {
		for (int j = 0; j < room_width; j++) {
			switch (roomTiles[i][j]) {
			case 'U':
				UexitX = i;
				UexitY = j;
				break;
			case 'D':
				DexitX = i;
				DexitY = j;
				break;
			case 'L':
				LexitX = i;
				LexitY = j;
				break;
			case 'R':
				RexitX = i;
				RexitY = j;
				break;
			}
		}
	}

#ifdef _DEBUG
	cout << "Name: " << room_name << endl;
	cout << "Type: " << room_type << endl;
	cout << "Width: " << room_width << ", " << "Height: " << room_height << endl;
	cout << "Center: " << CenterX << "," << CenterY << endl;
	cout << "Exit U: " << doorU << " | " << UexitX << "," << UexitY << endl;
	cout << "Exit D: " << doorD << " | " << DexitX << "," << DexitY << endl;
	cout << "Exit L: " << doorL << " | " << LexitX << "," << LexitY << endl;
	cout << "Exit R: " << doorR << " | " << RexitX << "," << RexitY << endl;
	cout << endl;
	printLayoutTiles();
	cout << endl;
#endif

}

DungeonRoom::~DungeonRoom()
{
	// Clear tilemap
	delete tilemap;
}

void DungeonRoom::CreateTilemap() {
	// Create the tilemap for the room
	tilemap = new Tilemap{ roomTiles, this };

	// Store tile size from tilemap for use later
	tilesize = tilemap->getTileSize();
}

void DungeonRoom::render(SDL_Renderer* rend) const {
	// Render tilemap of current room
	tilemap->render(rend);
}

void DungeonRoom::spawnEnemies() {
	for (auto i : roomEnemies) {
		float realX = i.pos.getX() * tilesize;
		float realY = i.pos.getY() * tilesize;
		Vector2D realPos = { (float)realX, (float)realY };
		enemyutils().spawn_enemy(i.name, realPos);
	}
}

int DungeonRoom::getAmountOfExits() {
	int counter = 0;
	if (doorU) counter++;
	if (doorD) counter++;
	if (doorL) counter++;
	if (doorR) counter++;
	return counter;
}

vector<char> DungeonRoom::getUnusedExits() {
	vector<char> availableExits;
	if (doorU && !linkU) availableExits.push_back('U');
	if (doorD && !linkD) availableExits.push_back('D');
	if (doorL && !linkL) availableExits.push_back('L');
	if (doorR && !linkR) availableExits.push_back('R');
	return availableExits;
}

char DungeonRoom::getRandomUnusedExit() {
	vector<char> availableExits;
	if (doorU && !linkU) availableExits.push_back('U');
	if (doorD && !linkD) availableExits.push_back('D');
	if (doorL && !linkL) availableExits.push_back('L');
	if (doorR && !linkR) availableExits.push_back('R');
	
	if (availableExits.size() > 0) {
		int minNum = 0;
		int maxNum = availableExits.size() - 1;
		int randomExit = rand() % (maxNum - minNum + 1) + minNum;
		return availableExits[randomExit];
	}
	else {
		return '-';
	}
}

Vector2D DungeonRoom::PositionAfterEntering(char exit) {
	switch (exit) {
		float x;
		float y;
	case 'U':
		x = UexitX * tilesize + (tilesize / 2);
		y = UexitY * tilesize + (tilesize / 2);
		// Offset to the next tile after the exit
		y = y + tilesize;
		return Vector2D{x , y };
		break;
	case 'D':
		x = DexitX * tilesize + (tilesize / 2);
		y = DexitY * tilesize + (tilesize / 2);
		// Offset to the next tile after the exit
		y = y - tilesize;
		return Vector2D{ x , y };
		break;
	case 'L':
		x = LexitX * tilesize + (tilesize / 2);
		y = LexitY * tilesize + (tilesize / 2);
		// Offset to the next tile after the exit
		x = x + tilesize;
		return Vector2D{ x , y };
		break;
	case 'R':
		x = RexitX * tilesize + (tilesize / 2);
		y = RexitY * tilesize + (tilesize / 2);
		// Offset to the next tile after the exit
		x = x - tilesize;
		return Vector2D{ x , y };
		break;
	case ' ':
		x = CenterX * tilesize + (tilesize / 2);
		y = CenterY * tilesize + (tilesize / 2);
		return Vector2D{ x , y };
		break;
	default:
		return Vector2D{ -1,-1 };
		break;
	}
}

#ifdef _DEBUG
void DungeonRoom::printLayoutTiles() {
	for (int i = 0; i < room_height; i++) {
		for (int j = 0; j < room_width; j++) {
			cout << roomTiles[j][i];
		}
		cout << endl;
	}
	cout << endl;
}
#endif