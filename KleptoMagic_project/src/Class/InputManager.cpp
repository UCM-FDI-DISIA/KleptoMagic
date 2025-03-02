#include "InputManager.h"
#include "SDL_gamecontroller.h"
#include <iostream>
#include <windows.h>  
#include <iomanip>    

void clearScreen() {
    COORD topLeft = {0, 0};
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO screen;
    DWORD written;

    GetConsoleScreenBufferInfo(console, &screen);
    FillConsoleOutputCharacterA(console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written);
    FillConsoleOutputAttribute(console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE, screen.dwSize.X * screen.dwSize.Y, topLeft, &written);
    SetConsoleCursorPosition(console, topLeft);
}

InputManager::InputManager() {}

InputManager::~InputManager() {
    shutdown();
}

bool InputManager::initialize() {
    if (SDL_Init(SDL_INIT_GAMECONTROLLER | SDL_INIT_EVENTS) < 0) {
        std::cerr << "SDL initialization error: " << SDL_GetError() << std::endl;
        return false;
    }

    connectController(0);  
    return true;
}

void InputManager::update() {
    SDL_GameControllerUpdate();
    setMovementVector();
    printInput();
    
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_CONTROLLERDEVICEADDED) {
            connectController(event.cdevice.which);
        } else if (event.type == SDL_CONTROLLERDEVICEREMOVED) {
            disconnectController();
        }
    }

    if (_controller == nullptr) {
        std::cout << "Gamepad not found. Waiting for connection..." << std::endl;
    }
}

bool InputManager::isAbilityButtonPressed() {
    if (_controller) {
        return SDL_GameControllerGetButton(_controller, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER) ||
               SDL_GameControllerGetButton(_controller, SDL_CONTROLLER_BUTTON_LEFTSHOULDER);
    }
    
    const Uint8* keystate = SDL_GetKeyboardState(NULL);
    return keystate[SDL_SCANCODE_F];
}

void InputManager::connectController(int index) {
    if (SDL_IsGameController(index)) {
        _controller = SDL_GameControllerOpen(index);
        if (_controller) {
            std::cout << "Gamepad connected: " << SDL_GameControllerName(_controller) << std::endl;
        }
    }
}

void InputManager::disconnectController() {
    if (_controller) {
        SDL_GameControllerClose(_controller);
        _controller = nullptr;
        std::cout << "Gamepad disconnected." << std::endl;
    }
}

void InputManager::setMovementVector() {
    if (_controller) {
        int lx = SDL_GameControllerGetAxis(_controller, SDL_CONTROLLER_AXIS_LEFTX);
        int ly = SDL_GameControllerGetAxis(_controller, SDL_CONTROLLER_AXIS_LEFTY);
        float norm_lx = lx / 32767.0f;
        float norm_ly = ly / 32767.0f;
        MovementVector = Vector2D(norm_lx, norm_ly).normalize();
    } else {
        const Uint8* keystate = SDL_GetKeyboardState(NULL);
        float x = keystate[SDL_SCANCODE_D] - keystate[SDL_SCANCODE_A];
        float y = keystate[SDL_SCANCODE_S] - keystate[SDL_SCANCODE_W];
        MovementVector = Vector2D(x, y).normalize();
    }
}

void InputManager::shutdown() {
    if (_controller) {
        SDL_GameControllerClose(_controller);
        _controller = nullptr;
    }
    SDL_Quit();
}

void InputManager::printInput() {
    std::cout << "MovementVector: " << MovementVector.getX() << ", " << MovementVector.getY() << std::endl;
    system("cls");
}