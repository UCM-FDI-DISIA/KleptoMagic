#include "DungeonRoom.h"
#include <iostream>
#include <fstream>

#include "./../utils/StringUtils.h"

DungeonRoom::DungeonRoom(string filename)
{
	cout << "CREATING ROOM: " << filename << endl;

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

	cout << endl << endl;

	// ifstream roomFile(filename);
}

DungeonRoom::~DungeonRoom()
{

}

void DungeonRoom::render() const {

}

void DungeonRoom::update() {

}