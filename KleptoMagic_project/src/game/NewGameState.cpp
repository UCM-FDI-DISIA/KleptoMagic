#include "NewGameState.h"
#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/NewInputHandler.h"
#include "../utils/Vector2D.h"

using ecs::Manager;
using namespace std;

NewGameState::NewGameState() {
#ifdef _DEBUG
    std::cout << "Nuevo NewGameState creado!" << std::endl;
#endif

    // Cargar el fondo
    background = new Texture(sdlutils().renderer(), "resources/images/mainMenu.png");

    // Cargar la textura del bot�n
    buttonTexture = new Texture(sdlutils().renderer(), "resources/images/play.png");
    exitButtonTexture = new Texture(sdlutils().renderer(), "resources/images/quit.png");

    float btnWidth = buttonTexture->width() / 4;
    float btnHeight = buttonTexture->height() / 4;

    float exitBtnWidth = exitButtonTexture->width() / 4;
    float exitBtnHeight = exitButtonTexture->height() / 4;

    // Posicion base
    float centerX = (sdlutils().width() - btnWidth) / 2;
    float baseY = sdlutils().height() * 0.50f;

    // Boton Play
    float playBtnX = centerX - 50;
    float playBtnY = baseY;
    startButton = new Button([this]() {
        releaseTime = SDL_GetTicks() + 100;
        }, Vector2D(playBtnX, playBtnY), Vector2D(btnWidth, btnHeight), buttonTexture);

    // Boton Exit
    float exitBtnX = centerX - 50;  // Ajuste lateral
    float exitBtnY = playBtnY + btnHeight - 12;  // Espaciado vertical

    exitButton = new Button([this]() {
        game().exitGame(); // Sale del juego directamente
        }, Vector2D(exitBtnX, exitBtnY), Vector2D(exitBtnWidth, exitBtnHeight), exitButtonTexture);

}

NewGameState::~NewGameState() {
    //delete background;
    //delete buttonTexture;
    //delete startButton;
}

void NewGameState::update() {
    bool exit = false;
    //auto& ihdlr = ih();


    sdlutils().resetTime();

    while (!exit) {
        //std::cout << "En el bucle de actualizaci�n de newgame" << std::endl;
        Uint32 startTime = sdlutils().currRealTime();

        // Actualizar eventos
        //ih().refresh();
        NewInputHandler::Instance()->update();

        // Actualizar bot�n (manejo de clic)
        startButton->update();
        exitButton->update();

        // Si han pasado 100ms y el usuario solt� el clic, cambiamos de estado
        if (releaseTime > 0 && SDL_GetTicks() > releaseTime && !(SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT))) {
            game().setState(Game::NEWROUND);
            exit = true;
        }

        // Limpiar pantalla
        sdlutils().clearRenderer();

        // Dibujar el fondo
        SDL_Rect destRect = { 0, 0, sdlutils().width(), sdlutils().height() };
        background->render(destRect);

        // Dibujar el bot�n
        startButton->render();
        exitButton->render();

        // Presentar la pantalla
        sdlutils().presentRenderer();

        if (startButton->isPressed()) { 
            exit = true;
#ifdef _DEBUG
            cout << "Play isPressed: : " << exit << endl;
#endif
        }
        else if (exitButton->isPressed()) {
            exit = true;
#ifdef _DEBUG
            cout << "Exit isPressed: : " << exit << endl;
#endif
        }

        Uint32 frameTime = sdlutils().currRealTime() - startTime;

        if (frameTime < 20)
            SDL_Delay(20 - frameTime);
    }
}

void NewGameState::enter() {
#ifdef _DEBUG
    std::cout << "Entrando en NewGameState" << std::endl;
#endif
}

void NewGameState::leave() {
#ifdef _DEBUG
    std::cout << "Saliendo en NewGameState" << std::endl;
#endif
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
