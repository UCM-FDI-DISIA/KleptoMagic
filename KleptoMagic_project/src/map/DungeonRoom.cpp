#include "DungeonRoom.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <random>

#include "./../utils/StringUtils.h"

DungeonRoom::DungeonRoom(string filename, roomType type) : room_type(type)
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
	roomTiles = vector<vector<char>>(room_height, vector<char>(room_width, 0));
	string line;
	int row = 0;
	while (getline(roomFile, line) && row < room_height) {
		stringstream ss(line);
		for (int i = 0; i < room_width; i++) {
			roomTiles[row][i] = line[i];
		}
		row++;
	}

	getline(roomFile, line); // Ignoring the blank line between both matrices in the file

	//Reading the second matrix in the file, AKA the spawn layout
	roomSpawns = vector<vector<char>>(room_height, vector<char>(room_width, 0));
	row = 0;
	while (getline(roomFile, line) && row < room_height) {
		stringstream ss(line);
		for (int i = 0; i < room_width; i++) {
			roomSpawns[row][i] = line[i];
		}
		row++;
	}

	// Find and store the center and exit locations
	if (room_type == roomType::ENTRANCE) {
		CenterX = room_width / 2;
		CenterY = room_height / 2;
	}
	for (int i = 0; i < room_height; i++) {
		for (int j = 0; j < room_width; j++) {
			switch (roomTiles[i][j]) {
			case 'U':
				UexitX = j;
				UexitY = i;
				break;
			case 'D':
				DexitX = j;
				DexitY = i;
				break;
			case 'L':
				LexitX = j;
				LexitY = i;
				break;
			case 'R':
				RexitX = j;
				RexitY = i;
				break;
			}
		}
	}

#ifdef _DEBUG
	cout << "Name: " << room_name << endl;
	cout << "Type: " << room_type << endl;
	cout << "Width: " << room_width << ", " << "Height: " << room_height << endl;
	if (room_type == roomType::ENTRANCE) {
		cout << "Center: " << CenterX << "," << CenterY << endl;
	}
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

}

void DungeonRoom::render() const {

}

void DungeonRoom::update() {

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

#ifdef _DEBUG
void DungeonRoom::printLayoutTiles() {
	for (int i = 0; i < room_height; i++) {
		for (int j = 0; j < room_width; j++) {
			cout << roomTiles[i][j];
		}
		cout << endl;
	}
	cout << endl;
}

void DungeonRoom::printLayoutSpawns() {
	for (int i = 0; i < room_height; i++) {
		for (int j = 0; j < room_width; j++) {
			cout << roomSpawns[i][j];
		}
		cout << endl;
	}
	cout << endl;
}
#endif