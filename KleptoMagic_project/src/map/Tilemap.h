#pragma once

#include "SDL.h"
#include <string>
#include <vector>

using namespace std;

enum TileType {
	FLOOR = 0,
	WALL = 1,
	HOLE = 2,
	EXIT_U = 3,
	EXIT_D = 4,
	EXIT_L = 5,
	EXIT_R = 6
};

class Tilemap {
public:
	Tilemap(vector<vector<char>> tilematrix);
	~Tilemap() = default;

	void render(SDL_Renderer* renderer);

	void setTile(int x, int y, TileType tiletype);
	void setXOffset(int xOffset);
	void setYOffset(int yOffset);

	int getTilemapWidth();
	int getTilemapHeight();
	int getXOffset();
	int getYOffset();
	int getTileSize();
	vector<vector<TileType>> getTileMap();

private:
	SDL_Texture* tileset;
	vector<vector<TileType>> tilemap;
	int xOffset = 0;
	int yOffset = 0;
	const int TILE_SIZE = 16;
};