#include "InputManager.h"
#include "SDL_gamecontroller.h"
#include <iostream>
#include <vector>
#include <windows.h>  // For using SetConsoleCursorPosition
#include <iomanip>    // For std::fixed and std::setprecision

InputManager::InputManager() {}

InputManager::~InputManager() {
    shutdown();
}

bool InputManager::initialize() {
    if (SDL_Init(SDL_INIT_GAMECONTROLLER) < 0) {
        std::cerr << "SDL initialization error: " << SDL_GetError() << std::endl;
        return false;
    }
    scanForControllers();
    return true;
}

void InputManager::update() {
    SDL_GameControllerUpdate();
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_CONTROLLERDEVICEADDED) {
            connectController(event.cdevice.which);
        } else if (event.type == SDL_CONTROLLERDEVICEREMOVED) {
            disconnectController(event.cdevice.which);
        }
    }
    
    if (SDL_GameControllerGetButton(controllers[0], SDL_CONTROLLER_BUTTON_RIGHTSHOULDER)) {
        
    }
    clearConsole();
    if (controllers.empty()) {
        std::cout << "Gamepad not found. Waiting for connection..." << std::endl;
    } else {
        for (size_t i = 0; i < controllers.size(); ++i) {
            printControllerState(i);
        }
    }
    printConnectedControllers();
}

void InputManager::connectController(int index) {
    if (SDL_IsGameController(index)) {
        SDL_GameController* newController = SDL_GameControllerOpen(index);
        if (newController) {
            controllers.push_back(newController);
            std::cout << "Gamepad connected: " << SDL_GameControllerName(newController) << std::endl;
        }
    }
}

void InputManager::disconnectController(int instanceID) {
    for (auto it = controllers.begin(); it != controllers.end(); ++it) {
        if (SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(*it)) == instanceID) {
            SDL_GameControllerClose(*it);
            controllers.erase(it);
            std::cout << "Gamepad disconnected." << std::endl;
            break;
        }
    }
}

void InputManager::scanForControllers() {
    for (int i = 0; i < SDL_NumJoysticks(); ++i) {
        connectController(i);
    }
}

void InputManager::printControllerState(size_t index) {
    SDL_GameController* controller = controllers[index];
    int lx = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX);
    int ly = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY);
    int rx = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTX);
    int ry = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTY);
    int rt = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_TRIGGERRIGHT);
    int lt = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_TRIGGERLEFT);

    // GetAxis returns Sint16 values, so we need to normalize them to be between -1 and 1
    float norm_lx = lx / 32767.0f;
    float norm_ly = ly / 32767.0f;
    float norm_rx = rx / 32767.0f;
    float norm_ry = ry / 32767.0f;
    float norm_rt = rt / 32767.0f;
    float norm_lt = lt / 32767.0f;

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Controller " << index + 1 << ": Left stick X = " << norm_lx << ", Y = " << norm_ly << std::endl;
    std::cout << "Controller " << index + 1 << ": Right stick X = " << norm_rx << ", Y = " << norm_ry << std::endl;
    std::cout << "Controller " << index + 1 << ": Right trigger = " << norm_rt << std::endl;
    std::cout << "Controller " << index + 1 << ": Left trigger = " << norm_lt << std::endl;

    // Check buttons
    std::cout << "Button South: " << (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_A) ? "Pressed" : "Released") << std::endl;
    std::cout << "Button East: " << (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_B) ? "Pressed" : "Released") << std::endl;
    std::cout << "Button West: " << (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_X) ? "Pressed" : "Released") << std::endl;
    std::cout << "Button North: " << (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_Y) ? "Pressed" : "Released") << std::endl;
    std::cout << "RB: " << (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER) ? "Pressed" : "Released") << std::endl;
    std::cout << "LB: " << (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_LEFTSHOULDER) ? "Pressed" : "Released") << std::endl;
    SDL_Joystick* joystick = SDL_GameControllerGetJoystick(controller);

    // WIP check if the controller has rumble support and how to trigger rumble on ability use
    std::cout << "Has rumble: " << (SDL_JoystickHasRumble(joystick) ? "Yes" : "No") << std::endl;
    if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER)) {
        SDL_GameControllerRumble(controller, 16000, 16000, 300);
    }
}

void InputManager::printConnectedControllers() {
    std::cout << "\nConnected Controllers:" << std::endl;
    for (size_t i = 0; i < controllers.size(); ++i) {
        std::cout << i + 1 << ". " << SDL_GameControllerName(controllers[i]) << std::endl;
    }
}

void InputManager::shutdown() {
    for (SDL_GameController* controller : controllers) {
        SDL_GameControllerClose(controller);
    }
    controllers.clear();
    SDL_Quit();
}

// Clear the console screen, so we can print the controller state in a clean way
// It deletes cout result from another parts of the code, so it's not recommended to use it apart from input debugging
void InputManager::clearConsole() {
    COORD topLeft = {0, 0};
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO screen;
    DWORD written;

    GetConsoleScreenBufferInfo(console, &screen);
    FillConsoleOutputCharacterA(console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written);
    FillConsoleOutputAttribute(console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE, screen.dwSize.X * screen.dwSize.Y, topLeft, &written);
    SetConsoleCursorPosition(console, topLeft);
}
