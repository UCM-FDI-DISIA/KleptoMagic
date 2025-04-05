#pragma once
#include <vector>
#include <iostream>
#include "Tilemap.h"
#include "../utils/Vector2D.h"
using namespace std;

enum roomType { ENTRANCE, REGULAR, SPECIAL, BOSS };

struct spawnData {
	Vector2D pos;
	string name;

	spawnData(Vector2D p, string n) : pos(p), name(n) {};
};

class DungeonRoom
{
private:
	// Number of horizontal tiles in the room
	int room_width;
	// Number of vertical tiles in the room
	int room_height;
	// Name of the room
	string room_name; 
	// Room type
	roomType room_type;
	// Tile matrix for the room
	vector<vector<char>> roomTiles; 
	// Entities vector for the room (enemies, decor, etc.)
	vector<spawnData> roomSpawns; 

	// Tilemap object
	Tilemap* tilemap;
	// Size of tilemap tiles
	int tilesize;

	// Whether or not an exit exists in any of the cardinal directions, and therefore can connect to another room through there
	bool doorU, doorD, doorL, doorR;
	// Whether or not an exit in any of the cardinal directions is currently locked and requires opening
	bool lockU, lockD, lockL, lockR; 

	// X and Y coordinates of the center (for entrance rooms)
	int CenterX = -1, CenterY = -1;
	// X and Y coordinates of the U exit
	int UexitX = -1, UexitY = -1;
	// X and Y coordinates of the D exit
	int DexitX = -1, DexitY = -1;
	// X and Y coordinates of the L exit
	int LexitX = -1, LexitY = -1;
	// X and Y coordinates of the R exit
	int RexitX = -1, RexitY = -1;
public:
	// Whether or not any of the exits in any of the cardinal directions are currently linked to another room in said direction
	// Compared to the other variables, these can be readily changed, as they are only used during floor generation and rendering (?)
	bool linkU, linkD, linkL, linkR;

	// ROOM FILE NAME FORMAT
	// xxxx_y_z_name.txt
	// 
	// xxxx -> available exits, as 0 or 1 (bool), ordered as Up, Down, Left, Right (eg. 0100 if the only exit it has is Down)
	// y -> room height (int)
	// z -> room width (int)
	// note: room shape must be an overall rectangle/square, if the actual room has an irregular shape
	// then blank spaces are filled in later in the actual layout: this is the size of the whole thing
	// Room type must be included as an argument too: enum {ENTRANCE, REGULAR, SPECIAL, BOSS} so it can be identified easier
	DungeonRoom(string filename, roomType type);
	~DungeonRoom();
	void CreateTilemap();
	virtual void render(SDL_Renderer* rend) const;

	// Returns room width in terms of tiles
	int getWidth() { return room_width; }; 
	// Returns room height in terms of tiles
	int getHeight() { return room_height; };
	// Returns room name
	string getName() { return room_name; };
	// Returns room type
	roomType getType() { return room_type; }
	// Returns the tile matrix
	vector<vector<char>> getRoomTiles() { return roomTiles; };
	// Returns the tilemap
	Tilemap* getTilemap() { return tilemap; };

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
	// Returns a vector holding all of the exits the room has not linked yet
	vector<char> getUnusedExits();
	// Returns a random exit available that isn't already connected to another room (U, D, L, R, N (none))
	char getRandomUnusedExit();
	// Returns the real in-world position the tile right after an exit of the room (used to place the player after they enter a room)
	Vector2D PositionAfterEntering(char exit);

#ifdef _DEBUG
	// FOR TESTING: prints room tile layout to console, the same way as it is stored
	void printLayoutTiles();
	// FOR TESTING: prints room spawns layout to console, the same way as it is stored
	void printLayoutSpawns();
#endif
	
};