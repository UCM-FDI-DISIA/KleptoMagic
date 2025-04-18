#include "Button.h"
#include "../sdlutils/SDLUtils.h"

Button::Button(std::function<void()> onClick, Vector2D position, Vector2D size, Texture* texture, const std::string& soundId)
    : _onClick(onClick), _position(position), _size(size), _texture(texture), _soundId(soundId) {}

void Button::initComponent() {
}

void Button::update() {
    //std::cout << "Actualizando el boton..." << std::endl;

    // Obtener el estado del mouse
    int x, y;
    Uint32 mouseState = SDL_GetMouseState(&x, &y);

    // Verificar si el mouse esta dentro del area del bot�n
    if (isInside(x, y)) {

        _texture->setAlpha(128); // Bajar la opcidad 

        if (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT)) { // Verifica si el boton izquierdo esta presionado
#ifdef _DEBUG
            std::cout << "Boton clickeado" << std::endl;
#endif
            if (!_soundId.empty()) {
                sdlutils().soundEffects().at(_soundId).play();
            }
            _isPressed = true;  // Marcamos que el boton fue presionado
            _onClick(); // Ejecuta la funcion cuando se hace clic
        }
        else {
            _isPressed = false;  // Si no esta presionado, marcamos como no presionado
        }
    }
    else _texture->setAlpha(255);
}

void Button::render() {
    if (_texture) {
        SDL_Rect dest = { (int)_position.getX(), (int)_position.getY(), (int)_size.getX(), (int)_size.getY() };
        _texture->render(dest); // Usa la funcion render de Texture
    }
}

void Button::handleEvent(const SDL_Event& event) {
    NewInputHandler::Instance()->update();
    //_inputHandler->update(event);
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        int x = event.button.x;
        int y = event.button.y;
        if (isInside(x, y)) {
            _isPressed = true;
        }
    }
    else if (event.type == SDL_MOUSEBUTTONUP) {
        int x = event.button.x;
        int y = event.button.y;
        if (isInside(x, y) && _isPressed) {
            _onClick();  // Solo ejecutar cuando el boton es soltado dentro del area
        }
        _isPressed = false;
    }
}

bool Button::isInside(int x, int y) const {
    //return x >= _position.getX() && x <= (_position.getX() + _size.getX()) &&
      //  y >= _position.getY() && y <= (_position.getY() + _size.getY());
    // Reducimos el área al 80% y la centramos
    float reducedWidth = _size.getX() * 0.7f;
    float reducedHeight = _size.getY() * 0.7f;

    float offsetX = (_size.getX() - reducedWidth) / 2.0f;
    float offsetY = (_size.getY() - reducedHeight) / 2.0f;

    float minX = _position.getX() + offsetX;
    float minY = _position.getY() + offsetY;
    float maxX = minX + reducedWidth;
    float maxY = minY + reducedHeight;

    return x >= minX && x <= maxX && y >= minY && y <= maxY;
}