#include "NewInputHandler.h"

bool NewInputHandler::init() {
    for (int i = 0; i < static_cast<int>(Action::COUNT); i++) {
        Action action = static_cast<Action>(i);
        _actionPressed[action] = false;
        _actionHeld[action] = false;
        _actionReleased[action] = false;
    }
    initializeController();
    return true;
}

void NewInputHandler::initializeController() {
    if (SDL_NumJoysticks() < 1) {
        _controller = nullptr;
        return;
    }

    _controller = SDL_GameControllerOpen(0);
    if (!_controller) {
        std::cerr << "Failed to open game controller: " << SDL_GetError() << std::endl;
    }
}

void NewInputHandler::update() {
    for (auto& pair : _actionPressed) pair.second = false;
    for (auto& pair : _actionReleased) pair.second = false;
    _movementVector.setX(0);
    _movementVector.setY(0);
    _anyKeyPressed = false;

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_KEYDOWN) {
            _anyKeyPressed = true;
        }
        else if (event.type == SDL_CONTROLLERBUTTONDOWN) {
            _anyKeyPressed = true;
        }

        switch (event.type) {
            case SDL_KEYDOWN:
                onKeyDown(event);
                break;
            case SDL_KEYUP:
                onKeyUp(event);
                break;
            case SDL_CONTROLLERBUTTONDOWN:
                onGameControllerButtonDown(event);
                break;
            case SDL_CONTROLLERBUTTONUP:
                onGameControllerButtonUp(event);
                break;
        }
    }
    UpdateMovementVector();
}

void NewInputHandler::onKeyDown(SDL_Event& event) {
    Action action;
    switch (event.key.keysym.scancode) {
        case SDL_SCANCODE_A: action = Action::MOVE_LEFT; break;
        case SDL_SCANCODE_D: action = Action::MOVE_RIGHT; break;
        case SDL_SCANCODE_W: action = Action::MOVE_UP; break;
        case SDL_SCANCODE_S: action = Action::MOVE_DOWN; break;
        case SDL_SCANCODE_SPACE: action = Action::SHOOT; break;
        case SDL_SCANCODE_ESCAPE: action = Action::QUIT; break;
        case SDL_SCANCODE_P: action = Action::PAUSE; break;
        case SDL_SCANCODE_F: action = Action::ABILITY; break;
        case SDL_SCANCODE_E: action = Action::INTERACT; break;
        
        default: return;
    }

    // If button was not held, set it to pressed
    if (!_actionHeld[action]) {
        _actionPressed[action] = true; // Pressed activates only once per press during current frame
    }

    _actionHeld[action] = true; // Held activates every frame while button is held
}

void NewInputHandler::UpdateMovementVector() {
    if (_actionHeld[Action::MOVE_LEFT])  _movementVector.setX(_movementVector.getX() - 1);
    if (_actionHeld[Action::MOVE_RIGHT]) _movementVector.setX(_movementVector.getX() + 1);
    if (_actionHeld[Action::MOVE_UP])    _movementVector.setY(_movementVector.getY() - 1);
    if (_actionHeld[Action::MOVE_DOWN])  _movementVector.setY(_movementVector.getY() + 1);
}

void NewInputHandler::onKeyUp(SDL_Event& event) {
    Action action;
    switch (event.key.keysym.scancode) {
        case SDL_SCANCODE_A: action = Action::MOVE_LEFT; break;
        case SDL_SCANCODE_D: action = Action::MOVE_RIGHT; break;
        case SDL_SCANCODE_W: action = Action::MOVE_UP; break;
        case SDL_SCANCODE_S: action = Action::MOVE_DOWN; break;
        case SDL_SCANCODE_SPACE: action = Action::SHOOT; break;
        case SDL_SCANCODE_ESCAPE: action = Action::QUIT; break;
        case SDL_SCANCODE_P: action = Action::PAUSE; break;
        case SDL_SCANCODE_F: action = Action::ABILITY; break;
        case SDL_SCANCODE_E: action = Action::INTERACT; break;
        default: return;
    }

    _actionReleased[action] = true; // Set to released only once per release during current frame
    _actionHeld[action] = false;    // Set to not held anymore
}

void NewInputHandler::onGameControllerButtonDown(SDL_Event& event) {
    Action action;
    switch (event.cbutton.button) {
        case SDL_CONTROLLER_BUTTON_DPAD_LEFT: action = Action::MOVE_LEFT; break;
        case SDL_CONTROLLER_BUTTON_DPAD_RIGHT: action = Action::MOVE_RIGHT; break;
        case SDL_CONTROLLER_BUTTON_DPAD_UP: action = Action::MOVE_UP; break;
        case SDL_CONTROLLER_BUTTON_DPAD_DOWN: action = Action::MOVE_DOWN; break;
        case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER: action = Action::SHOOT; break;
        case SDL_CONTROLLER_BUTTON_START: action = Action::PAUSE; break;
        case SDL_CONTROLLER_BUTTON_X: action = Action::ABILITY; break;
        case SDL_CONTROLLER_BUTTON_B: action = Action::INTERACT; break;
        
        default: return;
    }

    if (!_actionHeld[action]) {
        _actionPressed[action] = true;
    }
    _actionHeld[action] = true;
}

void NewInputHandler::onGameControllerButtonUp(SDL_Event& event) {
    Action action;
    switch (event.cbutton.button) {
        case SDL_CONTROLLER_BUTTON_DPAD_LEFT: action = Action::MOVE_LEFT; break;
        case SDL_CONTROLLER_BUTTON_DPAD_RIGHT: action = Action::MOVE_RIGHT; break;
        case SDL_CONTROLLER_BUTTON_DPAD_UP: action = Action::MOVE_UP; break;
        case SDL_CONTROLLER_BUTTON_DPAD_DOWN: action = Action::MOVE_DOWN; break;
        case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER: action = Action::SHOOT; break;
        case SDL_CONTROLLER_BUTTON_START: action = Action::PAUSE; break;
        case SDL_CONTROLLER_BUTTON_X: action = Action::ABILITY; break;
        case SDL_CONTROLLER_BUTTON_B: action = Action::INTERACT; break;
        default: return;
    }

    _actionReleased[action] = true;
    _actionHeld[action] = false;
}
