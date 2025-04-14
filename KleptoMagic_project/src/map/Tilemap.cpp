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
	render_advanced(renderer);
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

void Tilemap::render_advanced(SDL_Renderer* renderer) {
	for (int x = 0; x < getTilemapWidth(); x++) {
		for (int y = 0; y < getTilemapHeight(); y++) {
			SDL_Rect tileRect;
			tileRect.w = TILE_SIZE;
			tileRect.h = TILE_SIZE;
			tileRect.x = (x * TILE_SIZE) + xOffset;
			tileRect.y = (y * TILE_SIZE) + yOffset;
			
			render_tile_with_context(x, y, tileRect);

			//SDL_RenderPresent(renderer);
		}
	}
}

void Tilemap::render_tile_with_context(int x, int y, SDL_Rect dstRect) {

	double angle = 0;
	int srcX = 0;
	int srcY = 0;

	bool Fup = false;
	bool Fdown = false;
	bool Fleft = false;
	bool Fright = false;
	bool Wup = false;
	bool Wdown = false;
	bool Wleft = false;
	bool Wright = false;
	bool Hup = false;
	bool Hdown = false;
	bool Hleft = false;
	bool Hright = false;
	bool H_UL = false;
	bool H_LD = false;
	bool H_DR = false;
	bool H_RU = false;
	int floorCount = 0;
	int wallCount = 0;
	int holeCount = 0;
	TileType targettile;

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
		// check and count walls and floors
		if (y - 1 >= 0) {
			targettile = tilemap[x][y - 1];
			Fup = (targettile == TILE_FLOOR || targettile == TILE_HOLE);
			Wup = (targettile == TILE_WALL) ||
				(targettile == TILE_EXIT_U) || (targettile == TILE_EXIT_D) || (targettile == TILE_EXIT_L) || (targettile == TILE_EXIT_R);
		}
		if (y + 1 <= getTilemapHeight() - 1) {
			targettile = tilemap[x][y + 1];
			Fdown = (targettile == TILE_FLOOR || targettile == TILE_HOLE);
			Wdown = (targettile == TILE_WALL) ||
				(targettile == TILE_EXIT_U) || (targettile == TILE_EXIT_D) || (targettile == TILE_EXIT_L) || (targettile == TILE_EXIT_R);
		}
		if (x - 1 >= 0) {
			targettile = tilemap[x - 1][y];
			Fleft = (targettile == TILE_FLOOR || targettile == TILE_HOLE);
			Wleft = (targettile == TILE_WALL) ||
				(targettile == TILE_EXIT_U) || (targettile == TILE_EXIT_D) || (targettile == TILE_EXIT_L) || (targettile == TILE_EXIT_R);
		}
		if (x + 1 <= getTilemapWidth() - 1) {
			targettile = tilemap[x + 1][y];
			Fright = (targettile == TILE_FLOOR || targettile == TILE_HOLE);
			Wright = (targettile == TILE_WALL) ||
				(targettile == TILE_EXIT_U) || (targettile == TILE_EXIT_D) || (targettile == TILE_EXIT_L) || (targettile == TILE_EXIT_R);
		}

		if (Fup) floorCount++;
		if (Fdown) floorCount++;
		if (Fleft) floorCount++;
		if (Fright) floorCount++;

		if (Wup) wallCount++;
		if (Wdown) wallCount++;
		if (Wleft) wallCount++;
		if (Wright) wallCount++;

		// decide to place corner, regular wall, or multiwall based on surrounding floors (and holes, technically) and walls
		if (floorCount == 0) { // corner
			// is it a regular 2 wall corner, or does it connect more walls?
			if (wallCount <= 2) {
				srcX = 2;
				srcY = 0;
				// check for orientation
				if (Wup && Wleft) angle = 90;
				else if (Wleft && Wdown); // already oriented
				else if (Wdown && Wright) angle = 270;
				else if (Wright && Wup)angle = 180;
			}
			else {
				srcX = 3;
				srcY = 1;
				// check for orientation
				if (!Wup) angle = 270;
				if (!Wdown) angle = 90;
				if (!Wleft) angle = 180;
				if (!Wright); // already oriented
			}
		}
		else if (floorCount == 1) { // regular wall
			srcX = 1;
			srcY = 0;
			// check for orientation
			if (Fup) angle = 90;
			else if (Fdown) angle = 270;
			else if (Fleft); // already oriented
			else if (Fright) angle = 180;
		}
		else if (floorCount == 2) { // multiwall
			// decide whether it's an inner wall corner, or a separator wall
			if (Wup && Wdown) { // vertical separator
				srcX = 2;
				srcY = 1;
				angle = 90;
			}
			else if (Wleft && Wright) { // horizontal separator
				srcX = 2;
				srcY = 1;
			}
			else {
				srcX = 3;
				srcY = 0;
				// check for orientation
				if (Wup && Wleft) angle = 270;
				else if (Wleft && Wdown) angle = 180;
				else if (Wdown && Wright) angle = 90;
				else if (Wright && Wup); // already oriented
			}
		}
		else if (floorCount == 3) { // multiwall topper
			srcX = 1;
			srcY = 1;
			// check for orientation
			if (Wup) angle = 270;
			if (Wdown) angle = 90;
			if (Wleft) angle = 180;
			if (Wright); // already oriented
			
		}
		break;
	case TILE_HOLE:
		srcX = 0;
		srcY = 0;
		// check and count adjacent holes. first cardinals, then corners
		if (y - 1 >= 0) {
			targettile = tilemap[x][y - 1];
			Hup = (targettile == TILE_HOLE);
		}
		if (y + 1 <= getTilemapHeight() - 1) {
			targettile = tilemap[x][y + 1];
			Hdown = (targettile == TILE_HOLE);
		}
		if (x - 1 >= 0) {
			targettile = tilemap[x - 1][y];
			Hleft = (targettile == TILE_HOLE);
		}
		if (x + 1 <= getTilemapWidth() - 1) {
			targettile = tilemap[x + 1][y];
			Hright = (targettile == TILE_HOLE);
		}

		if ((y - 1 >= 0) && (x - 1 >= 0)) { // UL
			targettile = tilemap[x - 1][y - 1];
			H_UL = (targettile == TILE_HOLE);
		}
		if ((y + 1 <= getTilemapHeight() - 1) && (x - 1 >= 0)) { // LD
			targettile = tilemap[x - 1][y + 1];
			H_LD = (targettile == TILE_HOLE);
		}
		if ((y + 1 <= getTilemapHeight() - 1) && (x + 1 <= getTilemapWidth() - 1)) { // DR
			targettile = tilemap[x + 1][y + 1];
			H_DR = (targettile == TILE_HOLE);
		}
		if ((y - 1 >= 0) && (x + 1 <= getTilemapWidth() - 1)) { // RU
			targettile = tilemap[x + 1][y - 1];
			H_RU = (targettile == TILE_HOLE);
		}
		

		if (Hup) holeCount++;
		if (Hdown) holeCount++;
		if (Hleft) holeCount++;
		if (Hright) holeCount++;


		break;
	case TILE_EXIT_U:
		srcX = 0;
		srcY = 1;
		break;
	case TILE_EXIT_D:
		srcX = 0;
		srcY = 1;
		break;
	case TILE_EXIT_L:
		srcX = 0;
		srcY = 1;
		break;
	case TILE_EXIT_R:
		srcX = 0;
		srcY = 1;
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