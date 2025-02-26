#include "DungeonRoom.h"
#include <iostream>
#include <fstream>
#include <sstream>

#include "./../utils/StringUtils.h"

DungeonRoom::DungeonRoom(string filename)
{
	cout << "----------------------------------------------------------------------" << endl;
	cout << "CREATING ROOM: " << filename << endl;
	cout << "----------------------------------------------------------------------" << endl;
	
	// ROOM NAME FORMAT
	// xxxx_y_z_name.txt
	// 
	// xxxx -> available exits, as 0 or 1 (bool), ordered as Up, Down, Left, Right (eg. 0100 if the only exit it has is Down)
	// y -> room height (int)
	// z -> room width (int)
	// note: room shape must be an overall rectangle/square, if the actual room has an irregular shape
	// then blank spaces are filled in later in the actual layout: this is the size of the whole thing

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

	cout << "NAME:\t" << room_name << endl;
	cout << "SIZE:\t" << room_width << "x" << room_height << endl;
	cout << "EXITS:\t" << doorU << doorD << doorL << doorR << endl;

	cout << endl;

	ifstream roomFile;

	// ROOM TILE FORMAT
	// W -> wall
	// B -> blank (space outside walls, pitch black)
	// V -> void/hole
	// * -> walkable floor
	// U, D, L, R -> doors corresponding to up, down, left, right exits

	roomFile.open(filename);

	//cout << "Columnas: " << room_width << "\nFilas: " << room_height << "\n";

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
	cout << "TILES" << endl;
	for (int i = 0; i < room_height; i++) {
		for (int j = 0; j < room_width; j++) {
			cout << roomTiles[i][j];
		}
		cout << endl;
	}
	cout << endl;

	roomSpawns = vector<vector<char>>(room_height, vector<char>(room_width, 0));
	getline(roomFile, line); // ignoring blank line inbetween
	row = 0;
	while (getline(roomFile, line) && row < room_height) {
		stringstream ss(line);
		for (int i = 0; i < room_width; i++) {
			roomSpawns[row][i] = line[i];
		}
		row++;
	}
	cout << "SPAWNS" << endl;
	for (int i = 0; i < room_height; i++) {
		for (int j = 0; j < room_width; j++) {
			cout << roomSpawns[i][j];
		}
		cout << endl;
	}
	cout << endl;

}

DungeonRoom::~DungeonRoom()
{

}

void DungeonRoom::render() const {

}

void DungeonRoom::update() {

}