#include "RoomStorage.h"
#include "filesystem"

#include "DungeonRoom.h"

namespace fs = std::filesystem;

RoomStorage::RoomStorage()
{
	readAllRoomFiles();
}

RoomStorage::~RoomStorage()
{

}

void RoomStorage::readAllRoomFiles() 
{
	// Igual hacer un JSON con todas las direcciones de cada sala? 
	// Como tal no es necesario mientras no se cuele un archivo incorrecto en los directorios

	fs::directory_iterator Start;
	fs::directory_iterator End;

	Start = fs::directory_iterator{ R"(.\src\ProceduralPrototype\rooms\EntranceRooms)" };
	End = fs::directory_iterator{};
	for (auto Iter{ Start }; Iter != End; ++Iter) {
		//cout << Iter->path().string() << '\n';
		DungeonRoom* room = new DungeonRoom(Iter->path().string());
	}

	Start = fs::directory_iterator{R"(.\src\ProceduralPrototype\rooms\RegularRooms)"};
	End = fs::directory_iterator{};
	for (auto Iter{ Start }; Iter != End; ++Iter) {
		//cout << Iter->path().string() << '\n';
		DungeonRoom* room = new DungeonRoom(Iter->path().string());
	}

	Start = fs::directory_iterator{ R"(.\src\ProceduralPrototype\rooms\BossRooms)" };
	End = fs::directory_iterator{};
	for (auto Iter{ Start }; Iter != End; ++Iter) {
		//cout << Iter->path().string() << '\n';
		DungeonRoom* room = new DungeonRoom(Iter->path().string());
	}
}