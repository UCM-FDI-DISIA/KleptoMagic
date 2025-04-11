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

    // Cargar la textura del boton
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
        }, Vector2D(playBtnX, playBtnY), Vector2D(btnWidth, btnHeight), buttonTexture, "mainMenuButton");

    // Boton Exit
    float exitBtnX = centerX - 50;  // Ajuste lateral
    float exitBtnY = playBtnY + btnHeight;  // Espaciado vertical

    exitButton = new Button([this]() {
        game().exitGame(); // Sale del juego directamente
        }, Vector2D(exitBtnX, exitBtnY), Vector2D(exitBtnWidth, exitBtnHeight), exitButtonTexture, "mainMenuButton");

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
        //std::cout << "En el bucle de actualizacion de newgame" << std::endl;
        Uint32 startTime = sdlutils().currRealTime();

        // Actualizar eventos
        //ih().refresh();
        NewInputHandler::Instance()->update();

        // Actualizar boton (manejo de clic)
        startButton->update();
        exitButton->update();

        // Si han pasado 100ms y el usuario solto el clic, cambiamos de estado
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