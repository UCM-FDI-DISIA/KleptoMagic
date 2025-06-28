#include "Tilemap.h"
#include <iostream>
#include "DungeonRoom.h"
#include "../render/Camera.h"

using namespace std;

Tilemap::Tilemap(vector<vector<char>> tilematrix, DungeonRoom* dungeonroom) : room(dungeonroom) {

	// Instantiate the tilemap with the same size as the tile matrix
	tilemap = vector<vector<TileType>>(tilematrix.size(), vector<TileType>(tilematrix[0].size(), TILE_BLANK));

	// Prepare the tileset texture
	tileset = &sdlutils().images().at("tileset");
	tilesetTileSize = 32;
	tilesetWidth = 5;
	tilesetHeight = 5;

	// Prepare the room decor texture
	roomdecor = &sdlutils().images().at("room_decor");
	roomdecorTileSize = 32;
	roomdecorWidth = 4;
	roomdecorHeight = 2;

	// Set data of torch and door animations
	torchFrame = 0;
	doorFrame = 0;
	doorsOpen = false;
	setDoorClosed();
	
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
			case 'T':
				setTile(i, j, TILE_TORCH);
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
Tilemap::~Tilemap() {
	tilemap.clear();

	// 2. Resetear punteros (no eliminar porque no los poseemos)
	tileset = nullptr;
	roomdecor = nullptr;
	room = nullptr;

	// 3. Opcional: Resetear variables de estado por claridad
	torchFrame = 0;
	doorFrame = 0;
	doorsOpen = false;
	doorsOpening = false;
	doorsChanging = false;

	// Nota: No necesitamos eliminar tileset, roomdecor ni room porque:
	// - tileset y roomdecor son referencias obtenidas de sdlutils().images()
	// - room es pasado por parámetro y es propiedad de otra clase
}
void Tilemap::render(SDL_Renderer* renderer) {
	render_advanced(renderer);
}

void Tilemap::update() {
	if ((sdlutils().currRealTime() - lastTorchFrameChange > TorchFrameChangeInterval)) {
		lastTorchFrameChange = sdlutils().currRealTime();
		torchFrame++;
		if (torchFrame >= 4) {
			torchFrame = 0;
		}
	}

	if ((sdlutils().currRealTime() - lastDoorFrameChange > DoorFrameChangeInterval) && doorsChanging) {
		lastDoorFrameChange = sdlutils().currRealTime();
		if (doorsOpening) {
			doorFrame++;
			if (doorFrame >= 5) {
				setDoorOpen();
			}
		}
		else {
			doorFrame--;
			if (doorFrame <= 0) {
				setDoorClosed();
			}
		}
	}
}

void Tilemap::anim_doorsOpen() {
	setDoorClosed();
	doorsOpening = true;
	doorsChanging = true;
}
void Tilemap::anim_doorsClose() {
	setDoorOpen();
	doorsOpening = false;
	doorsChanging = true;
}
void Tilemap::setDoorClosed() {
	doorFrame = 0;
	doorsChanging = false;
}
void Tilemap::setDoorOpen() {
	doorFrame = 5;
	doorsChanging = false;

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
				SDL_SetRenderDrawColor(renderer, 0xFF, 0xAA, 0x00, 0xFF);
				break;
			case TILE_TORCH:
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
			tileRect.x = (x * TILE_SIZE) + xOffset - camOffset.getX();
			tileRect.y = (y * TILE_SIZE) + yOffset - camOffset.getY();
			
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
	case TILE_WALL: case TILE_TORCH:
		// check and count walls and floors
		if (y - 1 >= 0) {
			targettile = tilemap[x][y - 1];
			Fup = (targettile == TILE_FLOOR || targettile == TILE_HOLE);
			Wup = (targettile == TILE_WALL) ||
				(targettile == TILE_EXIT_U) || (targettile == TILE_EXIT_D) || (targettile == TILE_EXIT_L) || (targettile == TILE_EXIT_R) ||
				(targettile == TILE_TORCH);
		}
		if (y + 1 <= getTilemapHeight() - 1) {
			targettile = tilemap[x][y + 1];
			Fdown = (targettile == TILE_FLOOR || targettile == TILE_HOLE);
			Wdown = (targettile == TILE_WALL) ||
				(targettile == TILE_EXIT_U) || (targettile == TILE_EXIT_D) || (targettile == TILE_EXIT_L) || (targettile == TILE_EXIT_R) ||
				(targettile == TILE_TORCH);
		}
		if (x - 1 >= 0) {
			targettile = tilemap[x - 1][y];
			Fleft = (targettile == TILE_FLOOR || targettile == TILE_HOLE);
			Wleft = (targettile == TILE_WALL) ||
				(targettile == TILE_EXIT_U) || (targettile == TILE_EXIT_D) || (targettile == TILE_EXIT_L) || (targettile == TILE_EXIT_R) ||
				(targettile == TILE_TORCH);
		}
		if (x + 1 <= getTilemapWidth() - 1) {
			targettile = tilemap[x + 1][y];
			Fright = (targettile == TILE_FLOOR || targettile == TILE_HOLE);
			Wright = (targettile == TILE_WALL) ||
				(targettile == TILE_EXIT_U) || (targettile == TILE_EXIT_D) || (targettile == TILE_EXIT_L) || (targettile == TILE_EXIT_R) ||
				(targettile == TILE_TORCH);
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

		if (Hup) holeCount++;
		if (Hdown) holeCount++;
		if (Hleft) holeCount++;
		if (Hright) holeCount++;

		if (holeCount == 0) { // single lonely hole
			srcX = 3;
			srcY = 2;
		}
		else if (holeCount == 1) { // one-way hole topper
			srcX = 2;
			srcY = 2; 
			if (Hup); // already oriented
			else if (Hdown) angle = 180;
			else if (Hleft) angle = 270; 
			else if (Hright) angle = 90;
		}
		else if (holeCount == 2) { // could be a corner or could be a one-wide hole
			if (Hup && Hdown) { // vertical one-wide hole
				srcX = 2;
				srcY = 3;
			}
			else if (Hleft && Hright) { // horizontal one-wide hole
				srcX = 2;
				srcY = 3;
				angle = 90;
			}
			else { // corner
				srcX = 1;
				srcY = 2;
				if (Hup && Hleft) angle = 90;
				else if (Hleft && Hdown); // already oriented
				else if (Hdown && Hright) angle = 270;
				else if (Hright && Hup)angle = 180;
			}
		}
		else if (holeCount == 3) { // "wall" hole
			srcX = 0;
			srcY = 2;
			if (!Hup) angle = 90;
			else if (!Hdown) angle = 270;
			else if (!Hleft); // already oriented
			else if (!Hright) angle = 180;
		}
		else if (holeCount == 4) { // inner hole
			srcX = 0;
			srcY = 0;
		}

		break;
	case TILE_EXIT_U:
		srcX = 3;
		srcY = 3;
		angle = 180;
		break;
	case TILE_EXIT_D:
		srcX = 3;
		srcY = 3;
		break;
	case TILE_EXIT_L:
		srcX = 3;
		srcY = 3; 
		angle = 90;
		break;
	case TILE_EXIT_R:
		srcX = 3;
		srcY = 3;
		angle = 270;
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

	// draw torch if it is a torch wall
	if (tilemap[x][y] == TILE_TORCH) {
		SDL_Rect torchRect;
		torchRect.w = roomdecorTileSize;
		torchRect.h = roomdecorTileSize;
		torchRect.x = (torchFrame * roomdecorTileSize);
		torchRect.y = (roomdecorTileSize);
		roomdecor->render(torchRect, dstRect, angle + 90);
	}
	// draw door if it is an exit
	else if (tilemap[x][y] == TILE_EXIT_U || tilemap[x][y] == TILE_EXIT_D || tilemap[x][y] == TILE_EXIT_L || tilemap[x][y] == TILE_EXIT_R) {
		SDL_Rect doorRect;
		doorRect.w = roomdecorTileSize;
		doorRect.h = roomdecorTileSize;
		doorRect.x = (doorFrame * roomdecorTileSize);
		doorRect.y = 0;
		roomdecor->render(doorRect, dstRect, angle + 180);
	}
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
		case TILE_WALL : case TILE_TORCH:
			return 1;
			break;
		case TILE_HOLE:
			return 2;
			break;
		case TILE_EXIT_U: case TILE_EXIT_D: case TILE_EXIT_L: case TILE_EXIT_R:
			return 3;
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