#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#pragma once
#include <SDL.h>
#include <iostream>
#include "../utils/Vector2D.h"

/* Handles input from keyboard and gamepad*/
class InputManager {
public:

    InputManager();
    ~InputManager();

    Vector2D MovementVector;
    Vector2D AimingVector;

    bool initialize();
    void update();
    bool isAbilityButtonPressed();

private:
    SDL_GameController* _controller = nullptr;
    bool _isDebugOn;

    void connectController(int index);
    void disconnectController();
    void shutdown();
    void setMovementVector();
    void printInput();
};

#endif // INPUTMANAGER_H
