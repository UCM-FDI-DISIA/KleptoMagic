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

class NewInputHandler : public Singleton<NewInputHandler> {
    friend class Singleton<NewInputHandler>;
public:
    bool init();
    void update();
    
    bool isActionPressed(Action action) { return _actionPressed[action]; };
    bool isActionHeld(Action action) { return _actionHeld[action]; };
    bool isActionReleased(Action action) { return _actionReleased[action]; };
    bool isAnyKeyPressed() { return _anyKeyPressed; };
    Vector2D getMovementVector() { return _movementVector.normalize(); };
    
private:
    void onKeyDown(SDL_Event& event);
    void onKeyUp(SDL_Event& event);
    void onGameControllerButtonDown(SDL_Event& event);
    void onGameControllerButtonUp(SDL_Event& event);
    void UpdateMovementVector();
    void initializeController();

    std::unordered_map<Action, bool> _actionPressed;
    std::unordered_map<Action, bool> _actionHeld;
    std::unordered_map<Action, bool> _actionReleased;

    bool _anyKeyPressed;
    Vector2D _movementVector;
    SDL_GameController* _controller;
};

inline NewInputHandler& input() {
    return *NewInputHandler::Instance();
}

#endif // H_NEWINPUTHANDLER_H