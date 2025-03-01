#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#pragma once
#include <SDL.h>
#include <iostream>
#include <vector>

/*  WIP Input manager class is responsible for handling input from the gamepad and keyboard
    It returns button presses and axis vector from sticks and WASD keys
    All values are normalized to be between -1 and 1
*/
class InputManager {
public:
    InputManager();
    ~InputManager();

    bool initialize();
    void update();
    void scanForControllers();
    void processInput();
    void connectController(int index);
    void printControllerState(size_t index);
    void printConnectedControllers();
    void disconnectController(int instanceID);
    void shutdown();
    void clearConsole();
    std::string getDirection(float x, float y);

private:
    SDL_GameController* controller;
    std::vector<SDL_GameController*> controllers;
};

#endif // INPUTMANAGER_H
