#include "DungeonRoom.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <random>

#include "./../utils/StringUtils.h"

DungeonRoom::DungeonRoom(string filename, roomType type) : room_type(type)
{
	cout << "CREATING ROOM: " << filename << endl;

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
	int minNum = 0;
	int maxNum = availableExits.size() - 1;
	int randomRoom = rand() % (maxNum - minNum + 1) + minNum;
	return availableExits[randomRoom];
}

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