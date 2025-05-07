#pragma once
#ifndef BUTTON_H
#define BUTTON_H

#include "../ecs/ecs.h"
#include "../ecs/ecs_defs.h"
#include "../ecs/Component.h"
#include "EventHandler.h"
#include "../sdlutils/NewInputHandler.h"
#include "InputManager.h"
#include "../utils/Vector2D.h"
#include "../sdlutils/Texture.h"

#include <functional>
#include <SDL.h>

using OnClickCallback = std::function<void()>;

class Button : public ecs::Component, public EventHandler {
public:
    __CMPID_DECL__(ecs::cmp::BUTTON)

    Button(std::function<void()> onClick, Vector2D position, Vector2D size, Texture* texture, const std::string& soundId = "");
    void initComponent() override;
    void update() override;
    void render() override;
    void selectButton() { _isSelected = true; }
    void handleEvent(const SDL_Event& event) override;
    bool isPressed() const { return _isPressed; }
    bool isSelected() const { return _isSelected; }

private:
    bool isInside(int x, int y) const;
    std::function<void()> _onClick;
    Vector2D _position;
    Vector2D _size;
    Texture* _texture; 
    bool _isPressed = false;
    bool _isSelected = false;
    std::string _soundId;
};

#endif // BUTTON_H