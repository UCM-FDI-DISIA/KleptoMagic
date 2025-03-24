// This file is part of the course TPV2@UCM - Samir Genaim

#include <SDL.h>

#include "./../../src/sdlutils/sdlutils_demo.h"

#include "./../../src/sdlutils/InputHandler.h"
#include "./../../src/sdlutils/macros.h"
#include "./../../src/sdlutils/SDLUtils.h"

#include "./../ProceduralPrototype/RoomStorage.h"
#include "./../ProceduralPrototype/DungeonFloor.h"

using namespace std;

void procedural_prototype() {

	// Initialize the SDLGame singleton
	if (!SDLUtils::Init("Procedural Generation Prototype", 800, 600,
			"resources/config/procedural_prototype.resources.json")) {
		std::cerr << "Something went wrong while initializing SDLUtils!"
				<< std::endl;
		return;
	}

	// Initialize the input handler
	if (!InputHandler::Init()) {
		std::cerr << "Something went wrong while initializing InputHandler!"
				<< std::endl;
		return;

	}

	// reference to the SDLUtils Singleton. You could use it as a pointer as well,
	// I just prefer to use . instead of ->, it is just a matter of taste, nothing
	// else!
	//
	// you can also use the inline method sdlutils() that simply returns the value
	// of *SDLUtils::instance() --- it is defined at the end of SDLUtils.h
	//
	auto &sdl = *SDLUtils::Instance();

	//show the cursor
	sdl.showCursor();

	// store the 'renderer' in a local variable, just for convenience
	SDL_Renderer *renderer = sdl.renderer();


	// some coordinates
	auto winWidth = sdl.width();
	auto winHeight = sdl.height();

	// reference to the input handler (we could use a pointer, I just . rather than ->).
	// you can also use the inline method ih() that is defined in InputHandler.h
	auto &ih = *InputHandler::Instance();

	// a boolean to exit the loop
	bool exit_ = false;

	// --------------------
	// PROCEDURAL PROTOTYPE
	// --------------------

	RoomStorage* roomstorage = new RoomStorage();
	cout << "----------------------------------------" << endl;
	cout << "----------------------------------------" << endl;
	cout << "----------------------------------------" << endl;
	DungeonFloor* dungeonfloor = new DungeonFloor(10, 10, 10, 10, 20, roomstorage);

	// --------------------
	// 
	// --------------------

	while (!exit_) {
		Uint32 startTime = sdl.currRealTime();

		// update the event handler
		ih.refresh();

		// exit when any key is down
		if (ih.keyDownEvent())
			exit_ = true;

		// clear screen
		sdl.clearRenderer();

		// render items

		// present new frame
		sdl.presentRenderer();

		Uint32 frameTime = sdl.currRealTime() - startTime;

		if (frameTime < 20)
			SDL_Delay(20 - frameTime);
	}

	// Release InputHandler and then SDLUtils (initialization reverse order)
	if (InputHandler::HasInstance())
		InputHandler::Release();
	if (SDLUtils::HasInstance())
		SDLUtils::Release();

}

