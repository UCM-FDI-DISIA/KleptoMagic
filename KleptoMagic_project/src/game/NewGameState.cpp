#include "NewGameState.h"
#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/InputHandler.h"
#include "../utils/Vector2D.h"

#include "../map/DungeonFloor.h"
#include "../map/RoomStorage.h"
#include <conio.h>

using ecs::Manager;

NewGameState::NewGameState() {
        std::cout << "Nuevo NewGameState creado!" << std::endl;

        // --------------------
        // PROCEDURAL PROTOTYPE
        // --------------------

        /*
        RoomStorage* roomstorage = new RoomStorage();
        while (true) {
            DungeonFloor* dungeonfloor = new DungeonFloor(10, 10, 10, 10, 10, roomstorage, sdlutils().renderer());

            sdlutils().clearRenderer();
            dungeonfloor->render();
            sdlutils().presentRenderer();
            
            _getch(); // waits for any key press before retrying. this is a demo
        }
        */
        

        // --------------------
        // 
        // --------------------


    // Cargar el fondo
    background = new Texture(sdlutils().renderer(), "resources/images/background-provisional.png");

    // Cargar la textura del botón
    buttonTexture = new Texture(sdlutils().renderer(), "resources/images/play-button.png");

    // Posicionar el botón en el centro
    float btnWidth = buttonTexture->width();
    float btnHeight = buttonTexture->height();
    float btnX = (sdlutils().width() - btnWidth) / 2;
    float btnY = (sdlutils().height() - btnHeight) / 2;

    // Crear el botón con su callback
    startButton = new Button([this]() {
        game().setState(Game::NEWROUND);
        }, Vector2D(btnX, btnY), Vector2D(btnWidth, btnHeight), buttonTexture);
}

NewGameState::~NewGameState() {
    delete background;
    delete buttonTexture;
    delete startButton;
}

void NewGameState::update() {
    bool exit = false;
    auto& ihdlr = ih();

    sdlutils().resetTime();

    while (!exit) {
        //std::cout << "En el bucle de actualización de newgame" << std::endl;
        Uint32 startTime = sdlutils().currRealTime();

        // Actualizar eventos
        ih().refresh();

        // Actualizar botón (manejo de clic)
        startButton->update();

        // Limpiar pantalla
        sdlutils().clearRenderer();

        // Dibujar el fondo
        SDL_Rect destRect = { 0, 0, sdlutils().width(), sdlutils().height() };
        background->render(destRect);

        // Dibujar el botón
        startButton->render();

        // Presentar la pantalla
        sdlutils().presentRenderer();

        if (startButton->isPressed()) { 
            exit = true;
        }

        Uint32 frameTime = sdlutils().currRealTime() - startTime;

        if (frameTime < 20)
            SDL_Delay(20 - frameTime);
    }
}

void NewGameState::enter() {
    std::cout << "Entrando en NewGameState" << std::endl;
}

void NewGameState::leave() {
    std::cout << "Saliendo en NewGameState" << std::endl;
}

//#include "NewGameState.h"
//#include "../sdlutils/SDLUtils.h"
//#include "../sdlutils/InputHandler.h"
//
//
//using ecs::Manager;	
//
//NewGameState::NewGameState(){
//	background = new Texture(sdlutils().renderer(), "resources/images/background-provisional.png");
//	pressAnyKey = new Texture(sdlutils().renderer(), 
//		"Press any key to start",
//		sdlutils().fonts().at("ARIAL24"), 
//		build_sdlcolor(0x112233ff),
//		build_sdlcolor(0xffffffff));
//
//	x0 = (sdlutils().width() - pressAnyKey->width()) / 2;
//	y0 = (sdlutils().height() - pressAnyKey->height()) / 2;
//}
//NewGameState::~NewGameState() {
//
//}
//
//void NewGameState::update() {
//	
//	bool exit = false;
//	auto& ihdlr = ih();
//
//	// reset the time before starting - so we calculate correct
//	// delta-time in the first iteration
//	//
//	sdlutils().resetTime();
//
//	while (!exit) {
//		Uint32 startTime = sdlutils().currRealTime();
//
//		// update the event handler
//		ih().refresh();
//
//		// enter NewRoundState when any key is down
//		if (ih().keyDownEvent()) {
//			//fighterutils().reset_lives();
//			//here
//			game().setState(Game::NEWROUND);
//			exit = true;
//		}
//
//		// clear screen
//		sdlutils().clearRenderer();
//
//		// render Press Any Key
//		background->render(0, 0);
//		pressAnyKey->render(x0, y0);
//
//		// present new frame
//		sdlutils().presentRenderer();
//
//		Uint32 frameTime = sdlutils().currRealTime() - startTime;
//
//		if (frameTime < 20)
//			SDL_Delay(20 - frameTime);
//	}
//}
//
//void NewGameState::enter()
//{
//	//fighterutils().reset_lives();
//	std::cout << "Entrando en NewGameState" << std::endl;
//}
//
//void NewGameState::leave()
//{
//	std::cout << "Saliendo NewGameState" << std::endl;
//}
