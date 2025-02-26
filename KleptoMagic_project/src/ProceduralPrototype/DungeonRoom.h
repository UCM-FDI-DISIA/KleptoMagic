#pragma once
#include "vector"
#include "iostream"
using namespace std;

class DungeonRoom
{
private:
	int room_width;
	int room_height;
	string room_name;
	vector<vector<char>> roomTiles;
	vector<vector<char>> roomSpawns;

	bool doorU, doorD, doorL, doorR;
	bool lockU, lockD, lockL, lockR;
public:
	DungeonRoom(string filename);
	~DungeonRoom();
	virtual void render() const;
	virtual void update();

	int getWidth() { return room_width; };
	int getHeight() { return room_height; };
	string getName() { return room_name; };
	void printLayoutTiles();
	void printLayoutSpawns();

	
};