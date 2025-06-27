#include "NewGameState.h"
#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/NewInputHandler.h"
#include "../utils/Vector2D.h"
#include "../state/NewRoundState.h"

using ecs::Manager;
using namespace std;

NewGameState::NewGameState() {
    // Load textures for background and buttons
    background = new Texture(sdlutils().renderer(), "resources/images/mainMenu.png");
    buttonTexture = new Texture(sdlutils().renderer(), "resources/images/play.png");
    exitButtonTexture = new Texture(sdlutils().renderer(), "resources/images/quit.png");

    // Set the size of the buttons based on the texture size
    float btnWidth = buttonTexture->width() / 4;
    float btnHeight = buttonTexture->height() / 4;

    float exitBtnWidth = exitButtonTexture->width() / 4;
    float exitBtnHeight = exitButtonTexture->height() / 4;

    // Set the center position of the buttons
    float centerX = (sdlutils().width() - btnWidth) / 2;
    float baseY = sdlutils().height() * 0.50f;

    // Create the start button
    float playBtnX = centerX - 50;
    float playBtnY = baseY;
    startButton = new Button([this]() {game().pushState(new NewRoundState());}, Vector2D(playBtnX, playBtnY), Vector2D(btnWidth, btnHeight), buttonTexture, "mainMenuButton");

    // Create the exit button
    float exitBtnX = centerX - 50;  // Ajuste lateral
    float exitBtnY = playBtnY + btnHeight;  // Espaciado vertical
    exitButton = new Button([this]() {
        game().exitGame(); // Sale del juego directamente
        }, Vector2D(exitBtnX, exitBtnY), Vector2D(exitBtnWidth, exitBtnHeight), exitButtonTexture, "mainMenuButton");

    // Add buttons to the vector for easy management
    registerButtons();

    // If gamepad is connected, select the first button
    if (input().isControllerConnected() && buttons.size() > 0) {
        buttons[0]->selectButton();
        selectedButtonIndex = 0;
    }
}

NewGameState::~NewGameState() {
    delete background;
    delete buttonTexture;
    delete exitButtonTexture;

    buttons.clear();
}

void NewGameState::update() {
    // Render the background
    SDL_Rect destRect = { 0, 0, sdlutils().width(), sdlutils().height() };
    background->render(destRect);

    // Render the buttons
    for (auto& button : buttons) {
        button->render();
    }

     // Trigger update for all buttons
     for (auto& button : buttons) {
        button->update();
    }

    // Presentar la pantalla
    sdlutils().presentRenderer();
}

void NewGameState::enter() {
    return;
}

void NewGameState::leave() {
    return;
}

void NewGameState::registerButtons() {
    buttons.push_back(startButton);
    buttons.push_back(exitButton);
}

void NewGameState::selectNextButton() {
    if (buttons.empty()) return;

    selectedButtonIndex = (selectedButtonIndex + 1) % buttons.size();
}