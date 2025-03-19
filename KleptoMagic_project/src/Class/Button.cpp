
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
    // Renderizado del bot�n (puede usarse SDL para dibujarlo visualmente)
}

void Button::handleEvent(const SDL_Event& event) {
    _inputHandler->update(event);
}

bool Button::isInside(int x, int y) const {
    return x >= _position.getX() && x <= (_position.getX() + _size.getX()) &&
        y >= _position.getY() && y <= (_position.getY() + _size.getY());
}

//#include "Button.h"
//Button::Button(GameState* state, Texture* tex, Vector2D pos, int w, int h) : GameObject(state, tex) {
//	position = pos;
//	width = w;
//	height = h;
//	buttonBox = { static_cast<int>(pos.getX()), static_cast<int>(pos.getY()), w, h };
//}
//
//
//void Button::addCallback(OnClickCallback callback) {
//	callbacks.push_back(callback);
//}
//
//
//void Button::handleEvent(const SDL_Event& event) {
//	if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
//		SDL_Point point{ event.button.x, event.button.y };
//		if (SDL_PointInRect(&point, &buttonBox)) {
//#ifdef _DEBUG
//			std::cout << "Bot�n clickeado!" << std::endl;
//#endif
//			emit();
//		}
//	}
//}
//
//
//void Button::render() const {
//	SDL_Renderer* renderer = state->getGame()->getRenderer();
//	SDL_Texture* sdl_texture = texture->getSDLTexture();
//
//	SDL_Color buttonColor;
//	if (isHovered) {
//		buttonColor = { 255, 0 , 0 , 255 };
//	}
//	else {
//		buttonColor = { 255, 255 , 255 , 255 };
//	}
//
//	texture->render(buttonBox, buttonColor);
//}
//
//
//void Button::update() {
//	setButtonBox(static_cast<int>(position.getX()), static_cast<int>(position.getY()), width, height);
//	SDL_Point point;
//	SDL_GetMouseState(&point.x, &point.y);
//
//	isHovered = SDL_PointInRect(&point, &buttonBox);
//}
//
//void Button::emit() const
//{
//	for (OnClickCallback callback : callbacks) {
//		callback();
//	}
//}
//
//void Button::setButtonBox(int x, int y, int w, int h) {
//	buttonBox = { x, y, w, h };
//}
