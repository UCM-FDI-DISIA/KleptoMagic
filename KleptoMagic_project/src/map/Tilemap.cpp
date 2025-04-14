#include "Tilemap.h"
#include <iostream>
#include "DungeonRoom.h"

using namespace std;

Tilemap::Tilemap(vector<vector<char>> tilematrix, DungeonRoom* dungeonroom) : room(dungeonroom) {

	// Instantiate the tilemap with the same size as the tile matrix
	tilemap = vector<vector<TileType>>(tilematrix.size(), vector<TileType>(tilematrix[0].size(), TILE_BLANK));

	// Prepare the tileset texture
	tileset = &sdlutils().images().at("tileset");
	tilesetTileSize = 32;
	tilesetWidth = 5;
	tilesetHeight = 5;

	// Run through the matrix and translate chars to tile types within the tilemap itself
	for (int i = 0; i < tilematrix.size(); i++) {
		for (int j = 0; j < tilematrix[0].size(); j++) {
			switch (tilematrix[i][j]) {
			case 'B':
				setTile(i, j, TILE_BLANK);
				break;
			case '*':
				setTile(i, j, TILE_FLOOR);
				break;
			case 'W':
				setTile(i, j, TILE_WALL);
				break;
			case 'H':
				setTile(i, j, TILE_HOLE);
				break;
			case 'U':
				if (dungeonroom->linkU) {
					setTile(i, j, TILE_EXIT_U);
				}
				else {
					setTile(i, j, TILE_WALL);
				}
				break;
			case 'D':
				if (dungeonroom->linkD) {
					setTile(i, j, TILE_EXIT_D);
				}
				else {
					setTile(i, j, TILE_WALL);
				}
				break;
			case 'L':
				if (dungeonroom->linkL) {
					setTile(i, j, TILE_EXIT_L);
				}
				else {
					setTile(i, j, TILE_WALL);
				}
				break;
			case 'R':
				if (dungeonroom->linkR) {
					setTile(i, j, TILE_EXIT_R);
				}
				else {
					setTile(i, j, TILE_WALL);
				}
				break;
			}
		}
	}
}

void Tilemap::render(SDL_Renderer* renderer) {
	render_advanced();
}

void Tilemap::render_basic(SDL_Renderer* renderer) {
	for (int x = 0; x < getTilemapWidth(); x++) {
		for (int y = 0; y < getTilemapHeight(); y++) {
			SDL_Rect tileRect;
			tileRect.w = TILE_SIZE;
			tileRect.h = TILE_SIZE;
			tileRect.x = (x * TILE_SIZE) + xOffset;
			tileRect.y = (y * TILE_SIZE) + yOffset;

			switch (tilemap[x][y]) {
			case TILE_BLANK:
				SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
				break;
			case TILE_FLOOR:
				SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);
				break;
			case TILE_WALL:
				SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
				break;
			case TILE_HOLE:
				SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);
				break;
			default:
				SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
				break;
			}

			SDL_RenderDrawRect(renderer, &tileRect);
		}
	}
}

void Tilemap::render_advanced() {
	for (int x = 0; x < getTilemapWidth(); x++) {
		for (int y = 0; y < getTilemapHeight(); y++) {
			SDL_Rect tileRect;
			tileRect.w = TILE_SIZE;
			tileRect.h = TILE_SIZE;
			tileRect.x = (x * TILE_SIZE) + xOffset;
			tileRect.y = (y * TILE_SIZE) + yOffset;
			
			render_tile_with_context(x, y, tileRect);
		}
	}
}

void Tilemap::render_tile_with_context(int x, int y, SDL_Rect dstRect) {

	double angle = 0;
	int srcX = 0;
	int srcY = 0;

	switch (tilemap[x][y]) {
	case TILE_BLANK:
		srcX = 0;
		srcY = 0;
		break;
	case TILE_FLOOR:
		srcX = 0;
		srcY = 1;
		break;
	case TILE_WALL:
		srcX = 1;
		srcY = 0;
		break;
	case TILE_HOLE:
		srcX = 0;
		srcY = 0;
		break;
	default:
		break;
	}
	SDL_Rect srcRect;
	srcRect.w = tilesetTileSize;
	srcRect.h = tilesetTileSize;
	srcRect.x = (srcX * tilesetTileSize);
	srcRect.y = (srcY * tilesetTileSize);
	
	tileset->render(srcRect, dstRect, angle);
}

int Tilemap::checkCollision(int x, int y) {
	int xInTiles = (x / TILE_SIZE);
	int yInTiles = (y / TILE_SIZE);

	if (xInTiles < 0 || xInTiles > getTilemapWidth() - 1 || yInTiles < 0 || yInTiles > getTilemapHeight() - 1) {
		return 0;
	}
	else {
		switch (tilemap[xInTiles][yInTiles]) {
		case TILE_FLOOR:
			return 0;
			break;
		case TILE_BLANK:
			return 1;
			break;
		case TILE_WALL:
			return 1;
			break;
		case TILE_HOLE:
			return 2;
			break;
		default:
			return 0;
			break;
		}
	}
}

char Tilemap::checkExit(int x, int y) {
	int xInTiles = (x / TILE_SIZE);
	int yInTiles = (y / TILE_SIZE);

	if (xInTiles < 0 || xInTiles > getTilemapWidth() - 1 || yInTiles < 0 || yInTiles > getTilemapHeight() - 1) {
		return 0;
	}
	else {
		switch (tilemap[xInTiles][yInTiles]) {
		case TILE_EXIT_U:
			return 'U';
			break;
		case TILE_EXIT_D:
			return 'D';
			break;
		case TILE_EXIT_L:
			return 'L';
			break;
		case TILE_EXIT_R:
			return 'R';
			break;
		default:
			return ' ';
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
#ifdef _DEBUG
			cout << tilemap[i][j];
#endif
		}
		cout << endl;
	}
	cout << endl;
}