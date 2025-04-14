#pragma once

#include "SDL.h"
#include <string>
#include <vector>

class DungeonRoom;
class Texture;

using namespace std;

enum TileType {
	TILE_FLOOR = 0,
	TILE_WALL = 1,
	TILE_HOLE = 2,
	TILE_BLANK = 3,
	TILE_EXIT_U = 4,
	TILE_EXIT_D = 5,
	TILE_EXIT_L = 6,
	TILE_EXIT_R = 7
};

class Tilemap {
public:
	Tilemap(vector<vector<char>> tilematrix, DungeonRoom* dungeonroom);
	~Tilemap() = default;

	void render(SDL_Renderer* renderer);

	void render_basic(SDL_Renderer* renderer);
	void render_advanced();
	void render_tile_with_context(int x, int y, SDL_Rect dstRect);

	void setTile(int x, int y, TileType tiletype);
	void setXOffset(int offset) { xOffset = offset; };
	void setYOffset(int offset) { yOffset = offset; };

	int checkCollision(int x, int y);
	char checkExit(int x, int y);

	int getTilemapWidth() { return tilemap.size(); };
	int getTilemapHeight() { return tilemap[0].size(); };
	int getXOffset() { return xOffset; };
	int getYOffset() { return yOffset; };
	int getTileSize() { return TILE_SIZE; };
	vector<vector<TileType>> getTileMap() { return tilemap; };

	void printTilemap();

private:
	Texture* tileset;
	vector<vector<TileType>> tilemap;
	int xOffset = 0;
	int yOffset = 0;
	const int TILE_SIZE = 50;
	int tilesetTileSize;
	int tilesetWidth;
	int tilesetHeight;

	DungeonRoom* room;
};