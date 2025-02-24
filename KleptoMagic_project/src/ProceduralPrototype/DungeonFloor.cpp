#include "DungeonFloor.h"

DungeonFloor::DungeonFloor()
{

}

DungeonFloor::~DungeonFloor()
{

}

void DungeonFloor::render() const {
	for (int i = 0; i < roomLayout.size(); i++) {
		for (int j = 0; i < roomLayout[i].size(); i++) {
			cout << roomLayout[i][j];
		}
		cout << endl;
	}
}

void DungeonFloor::update() {

}