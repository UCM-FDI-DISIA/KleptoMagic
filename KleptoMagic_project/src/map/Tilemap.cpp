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