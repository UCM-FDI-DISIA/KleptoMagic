#include "RoomStorage.h"
#include "filesystem"

namespace fs = std::filesystem;

RoomStorage::RoomStorage()
{
	readAllRooms_Regular();
}

RoomStorage::~RoomStorage()
{

}

void RoomStorage::readAllRooms_Regular() 
{
	// Igual hacer un JSON con todas las direcciones de cada sala
	fs::directory_iterator Start{R"(.\src\ProceduralPrototype\rooms\RegularRooms)"};
	fs::directory_iterator End{};

	for (auto Iter{ Start }; Iter != End; ++Iter) {
		cout << Iter->path().string() << '\n';
	}
}