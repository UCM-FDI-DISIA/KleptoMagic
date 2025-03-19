#pragma once
#ifndef BUTTON_H
#define BUTTON_H

#include "../ecs/ecs.h"
#include "../ecs/ecs_defs.h"
#include "../ecs/Component.h"
#include "EventHandler.h"
#include "../sdlutils/InputHandler.h"
#include "InputManager.h"
#include "../utils/Vector2D.h"
#include <functional>
#include <SDL.h>
using OnClickCallback = std::function<void()>;

class Button : public ecs::Component, public EventHandler {
public:
    __CMPID_DECL__(ecs::cmp::GAMECTRL)

        Button(std::function<void()> onClick, Vector2D position, Vector2D size);
    void initComponent() override;
    void update() override;
    void render() override;
    void handleEvent(const SDL_Event& event) override;

private:
    bool isInside(int x, int y) const;
    std::function<void()> _onClick;
    Vector2D _position;
    Vector2D _size;
    InputHandler* _inputHandler;
};

#endif // BUTTON_H