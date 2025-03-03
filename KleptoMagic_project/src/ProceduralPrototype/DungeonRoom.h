#pragma once
#include "vector"
#include "iostream"
using namespace std;

class DungeonRoom
{
private:
	// Number of horizontal tiles in the room
	int room_width;
	// Number of vertical tiles in the room
	int room_height;
	// Name of the room
	string room_name; 
	// Tile matrix for the room
	vector<vector<char>> roomTiles; 
	// Spawns matrix for the room (objects and entities to be spawned in the room on first load)
	vector<vector<char>> roomSpawns; 

	// Whether or not an exit exists in any of the cardinal directions, and therefore can connect to another room through there
	bool doorU, doorD, doorL, doorR; 
	// Whether or not an exit in any of the cardinal directions is currently locked and requires opening
	bool lockU, lockD, lockL, lockR; 
public:
	// ROOM FILE NAME FORMAT
	// xxxx_y_z_name.txt
	// 
	// xxxx -> available exits, as 0 or 1 (bool), ordered as Up, Down, Left, Right (eg. 0100 if the only exit it has is Down)
	// y -> room height (int)
	// z -> room width (int)
	// note: room shape must be an overall rectangle/square, if the actual room has an irregular shape
	// then blank spaces are filled in later in the actual layout: this is the size of the whole thing
	DungeonRoom(string filename);
	~DungeonRoom();
	virtual void render() const;
	virtual void update();

	// Returns room width in terms of tiles
	int getWidth() { return room_width; }; 
	// Returns room height in terms of tiles
	int getHeight() { return room_height; };
	// Returns room name
	string getName() { return room_name; };
	// FOR TESTING: prints room tile layout to console, the same way as it is stored
	void printLayoutTiles();
	// FOR TESTING: prints room spawns layout to console, the same way as it is stored
	void printLayoutSpawns();

	
};