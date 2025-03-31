#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include "DungeonRoom.h"
#include "RoomStorage.h"

#include "vector"
#include "iostream"
using namespace std;

// Struct for convenient usage of room positions. Only used in the generation algorithm
struct roomPos {
	int x;
	int y;
	bool operator==(roomPos const& other) const
	{
		return (this->x == other.x && this->y == other.y);
	}
	bool operator!=(roomPos const& other) const
	{
		return (this->x != other.x || this->y != other.y);
	}
}; 


class DungeonFloor
{
private:
	// In terms of the room matrix
	int floor_width; 
	// In terms of the room matrix
	int floor_height; 
	// Room matrix
	vector<vector<DungeonRoom*>> floorLayout;
	// Pointer to room storage
	RoomStorage* roomstorage;
	// X coordinate of starting room
	int startX;
	// Y coordinate of starting room
	int startY;
	// X coordinate of room the player is in
	int currentX;
	// Y coordinate of room the player is in
	int currentY;

	SDL_Renderer* renderer;

public:
	// minWidth / minHeight / maxWidth / maxHeight : 
	// - Minimum and maximum width and height of the floor layout matrix. The real size will be picked randomly within these. Reccomended a minimum of 10 width and height.
	// 
	// numRooms
	// - Dictates how many REGULAR rooms will make up the main path of the dungeon. 
	// 
	// roomstorage 
	// - A pointer towards the room storage is required, which should be constructed on game start.
	DungeonFloor(int minWidth, int minHeight, int maxWidth, int maxHeight, int numRooms, RoomStorage* storage, SDL_Renderer* rend);
	~DungeonFloor();

	// Render and Update are not needed.

	// Generates the whole floor based on the parameters given to the constructor.
	// May take more than one attempt, but due to how fast it is for reasonable floor sizes along with the rarity of
	// the only fatal error (looping into itself so it has no way to continue generating rooms in the line, and can't
	// reach the desired amount of rooms), it shouldn't take very long.
	void GenerateFloor(int minWidth, int minHeight, int maxWidth, int maxHeight, int numRooms);

	// Returns the X coordinate for the floor's starting room
	int getStartX() { return startX; };
	// Returns the Y coordinate for the floor's starting room
	int getStartY() { return startY; };

	// Renders the dungeon floor: i.e. renders the current room
	void render() const;

	// GENERATION ONLY: 
	// Checks for the existance of rooms or out of bounds for the cells above, below, left and right of the given coordinates. 
	// Returns a vector with the ones that are occupied in the form of chars (U, D, L, R)
	vector<char> CheckSpaceAroundRoom(int x, int y);
	// GENERATION ONLY: 
	// Checks for exits pointing towards the given coordinates within adjacent rooms to the space.
	// Returns a vector with the ones that should be filled by the room about to be placed in that spot (U, D, L, R)
	vector<char> ExitsToFillForSpace(int x, int y);
	// GENERATION ONLY: 
	// Links the exits given in the exits vector of the room located at the given coordinates of corresponding adjacent rooms,
	// ensuring those adjacent rooms also have their links updated to match.
	void LinkExitsAtPosition(int x, int y, vector<char> exits);
	// GENERATION ONLY: 
	// Small method used by the algorithm in the process of locating special room positions on empty exits after the basic floor plan is done. 
	void addPos(roomPos pos, vector<roomPos>& locations);

#ifdef _DEBUG
	// DEBUG ONLY: 
	// Prints a simple version of the floor layout only showing where rooms are.
	void PrintFloorLayout_Simple();
	// DEBUG ONLY: 
	// Prints a complex version of the floor layout, showing the exits of each room and whether they are linked or not.
	void PrintFloorLayout_Detailed();
#endif

};