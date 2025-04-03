#include "Tilemap.h"
#include <iostream>

using namespace std;

Tilemap::Tilemap(vector<vector<char>> tilematrix) {

	// Instantiate the tilemap with the same size as the tile matrix
	tilemap = vector<vector<TileType>>(tilematrix.size(), vector<TileType>(tilematrix[0].size(), BLANK));

	// Run through the matrix and translate chars to tile types within the tilemap itself
	for (int i = 0; i < tilematrix.size(); i++) {
		for (int j = 0; j < tilematrix[0].size(); j++) {
			switch (tilematrix[i][j]) {
			case 'B':
				setTile(i, j, BLANK);
				break;
			case '*':
				setTile(i, j, FLOOR);
				break;
			case 'W':
				setTile(i, j, WALL);
				break;
			case 'H':
				setTile(i, j, HOLE);
				break;
			case 'U':
				setTile(i, j, EXIT_U);
				break;
			case 'D':
				setTile(i, j, EXIT_D);
				break;
			case 'L':
				setTile(i, j, EXIT_L);
				break;
			case 'R':
				setTile(i, j, EXIT_R);
				break;
			}
		}
	}
}

void Tilemap::render(SDL_Renderer* renderer) {
	for (int x = 0; x < getTilemapWidth(); x++) {
		for (int y = 0; y < getTilemapHeight(); y++) {
			SDL_Rect tileRect;
			tileRect.w = TILE_SIZE;
			tileRect.h = TILE_SIZE;
			tileRect.x = (x * TILE_SIZE) + xOffset;
			tileRect.y = (y * TILE_SIZE) + yOffset;

			switch (tilemap[x][y]) {
			case BLANK:
				break;
			case FLOOR:
				SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF); // replace later with actual texture
				break;
			case WALL:
				SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF); // replace later with actual texture
				break;
			case HOLE:
				SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF); // replace later with actual texture
				break;
			default:
				SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF); // replace later with actual texture
				break;
			}

			SDL_RenderDrawRect(renderer, &tileRect);
		}
	}
}

int Tilemap::checkCollision(int x, int y) {
	int xInTiles = x / TILE_SIZE;
	int yInTiles = y / TILE_SIZE;
	if (x < 0 || x > getTilemapWidth() || y < 0 || y > getTilemapHeight()) {
		return 0;
	}
	else {
		switch (tilemap[xInTiles][yInTiles]) {
		case FLOOR:
			return 0;
			break;
		case BLANK:
			return 1;
			break;
		case WALL:
			return 1;
			break;
		case HOLE:
			return 2;
			break;
		default:
			return 0;
			break;
		}
	}
}

void Tilemap::setTile(int x, int y, TileType type) {
	if (!((x < 0 || x > getTilemapWidth()) && (y < 0 || y > getTilemapHeight()))) {
		tilemap[x][y] = type;
	}
}

void Tilemap::printTilemap() {
	for (int i = 0; i < getTilemapWidth(); i++) {
		for (int j = 0; j < getTilemapHeight(); j++) {
			cout << tilemap[i][j];
		}
		cout << endl;
	}
	cout << endl;
}