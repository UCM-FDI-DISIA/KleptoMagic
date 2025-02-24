#include "DungeonFloor.h"

DungeonFloor::DungeonFloor(int floorW, int floorH) : floor_width(floorW), floor_height(floorH)
{

}

DungeonFloor::~DungeonFloor()
{

}

void DungeonFloor::render() const {
	for (int i = 0; i < floorLayout.size(); i++) {
		for (int j = 0; i < floorLayout[i].size(); i++) {
			cout << floorLayout[i][j];
		}
		cout << endl;
	}
}

void DungeonFloor::update() {

}