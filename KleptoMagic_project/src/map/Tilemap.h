#pragma once

#include "SDL.h"
#include <string>
#include <vector>

using namespace std;

enum TileType {
	BLANK = 0,
	FLOOR = 1,
	WALL = 2,
	HOLE = 3,
	EXIT_U = 4,
	EXIT_D = 5,
	EXIT_L = 6,
	EXIT_R = 7
};

class Tilemap {
public:
	Tilemap(vector<vector<char>> tilematrix);
	~Tilemap() = default;

	void render(SDL_Renderer* renderer);

	void setTile(int x, int y, TileType tiletype);
	void setXOffset(int offset) { xOffset = offset; };
	void setYOffset(int offset) { yOffset = offset; };

	int getTilemapWidth() { return tilemap.size(); };
	int getTilemapHeight() { return tilemap[0].size(); };
	int getXOffset() { return xOffset; };
	int getYOffset() { return yOffset; };
	int getTileSize() { return TILE_SIZE; };
	vector<vector<TileType>> getTileMap() { return tilemap; };

	void printTilemap();

private:
	SDL_Texture* tileset;
	vector<vector<TileType>> tilemap;
	int xOffset = 0;
	int yOffset = 0;
	const int TILE_SIZE = 16;
};