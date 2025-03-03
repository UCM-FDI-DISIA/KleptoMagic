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
	// Whether or not any of the exits in any of the cardinal directions are currently linked to another room in said direction
	// Compared to the other variables, these can be readily changed, as they are only used during floor generation
	bool linkU, linkD, linkL, linkR;

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

	// Returns doorU
	bool hasExitUp() { return doorU; };
	// Returns doorD
	bool hasExitDown() { return doorD; };
	// Returns doorL
	bool hasExitLeft() { return doorL; };
	// Returns doorR
	bool hasExitRight() { return doorR; };

	// Returns lockU
	bool isLockedUp() { return lockU; };
	// Returns lockD
	bool isLockedDown() { return lockD; };
	// Returns lockL
	bool isLockedLeft() { return lockL; };
	// Returns lockR
	bool isLockedRight() { return lockR; };

	// Returns the total amount of exits the room has
	int getAmountOfExits();
	// Returns a random exit available that isn't already connected to another room (U, D, L, R, N (none))
	char getRandomUnusedExit();

	// FOR TESTING: prints room tile layout to console, the same way as it is stored
	void printLayoutTiles();
	// FOR TESTING: prints room spawns layout to console, the same way as it is stored
	void printLayoutSpawns();

	
};