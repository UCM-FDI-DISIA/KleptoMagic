#include "Button.h"
Button::Button(std::function<void()> onClick, Vector2D position, Vector2D size)
    : _onClick(onClick), _position(position), _size(size) {}

void Button::initComponent() {
    _inputHandler = &ih();
}

void Button::update() {
    if (_inputHandler->mouseButtonDownEvent()) {
        auto mousePos = _inputHandler->getMousePos();
        if (isInside(mousePos.first, mousePos.second)) {
            _onClick();
        }
    }
}

void Button::render() {
    // Renderizado del botón (puede usarse SDL para dibujarlo visualmente)
}

void Button::handleEvent(const SDL_Event& event) {
    _inputHandler->update(event);
}

bool Button::isInside(int x, int y) const {
    return x >= _position.getX() && x <= (_position.getX() + _size.getX()) &&
        y >= _position.getY() && y <= (_position.getY() + _size.getY());
}