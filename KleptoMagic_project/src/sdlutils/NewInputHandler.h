#ifndef H_NEWINPUTHANDLER_H
#define H_NEWINPUTHANDLER_H

#include <SDL.h>
#include <vector>
#include <unordered_map>

#include "../utils/Singleton.h"
#include "../utils/Vector2D.h"


enum class Action {
    MOVE_LEFT, 
    MOVE_RIGHT, 
    MOVE_UP, 
    MOVE_DOWN, 
    SHOOT, 
    PAUSE, 
    QUIT, 
    ABILITY, 
    INTERACT,
    COUNT
};

enum class RumbleType {
    TAP,
    BUZZ,
    LONG
};

class NewInputHandler : public Singleton<NewInputHandler> {
    friend class Singleton<NewInputHandler>;
public:
    bool init();
    void update();
    bool isControllerConnected() { return _controller != nullptr; };
    
    bool isActionPressed(Action action) { return _actionPressed[action]; };
    bool isActionHeld(Action action) { return _actionHeld[action]; };
    bool isActionReleased(Action action) { return _actionReleased[action]; };
    bool isAnyKeyPressed() { return _anyKeyPressed; };
    Vector2D getMovementVector() { return _movementVector.normalize(); };
    Vector2D getAimVector() { return _aimVector.normalize(); };
    
private:
    void initializeController();
    void onKeyDown(SDL_Event& event);
    void onKeyUp(SDL_Event& event);
    void onGameControllerButtonDown(SDL_Event& event);
    void onGameControllerButtonUp(SDL_Event& event);
    void onGameControllerAxisMotion(SDL_Event& event);
    void UpdateMovementVector();
    void UpdateAimVector(Vector2D playerPosition);
    void triggerRumble(RumbleType type);

    std::unordered_map<Action, bool> _actionPressed;
    std::unordered_map<Action, bool> _actionHeld;
    std::unordered_map<Action, bool> _actionReleased;
    Vector2D _leftStickVector, _rightStickVector;
    float _leftTriggerValue, _rightTriggerValue;
    float _rawLeftStickX, _rawLeftStickY, _rawRightStickX, _rawRightStickY;
    float _stickDeadZone = 0.1f; // Dead zone for left and right sticks

    bool _anyKeyPressed;
    Vector2D _movementVector, _aimVector;
    SDL_GameController* _controller;
};

inline NewInputHandler& input() {
    return *NewInputHandler::Instance();
}

#endif // H_NEWINPUTHANDLER_H