#include "DungeonRoom.h"
#include <iostream>
#include <fstream>

#include "./../utils/StringUtils.h"

DungeonRoom::DungeonRoom(string filename)
{
	cout << "CREATING ROOM: " << filename << endl;

	vector<string> filename_string_separated = splitStringByDelimiter(filename, "\\");
	string actual_filename = filename_string_separated[filename_string_separated.size() - 1];
	vector<string> room_data_from_filename = splitStringByDelimiter(actual_filename, "_");
	cout << "Room's exits: " << room_data_from_filename[0] << endl;
	cout << "Room's height: " << room_data_from_filename[1] << endl;
	cout << "Room's width: " << room_data_from_filename[2] << endl;
	cout << "Room's name: " << room_data_from_filename[3] << endl;
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