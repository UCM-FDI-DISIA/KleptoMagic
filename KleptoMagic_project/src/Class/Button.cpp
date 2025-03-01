#include "Button.h"
Button::Button(GameState* state, Texture* tex, Vector2D pos, int w, int h) : GameObject(state, tex) {
	position = pos;
	width = w;
	height = h;
	buttonBox = { static_cast<int>(pos.getX()), static_cast<int>(pos.getY()), w, h };
}


void Button::addCallback(OnClickCallback callback) {
	callbacks.push_back(callback);
}


/*void Button::handleEvent(const SDL_Event& event) {
	if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
		SDL_Point point{ event.button.x, event.button.y };
		if (SDL_PointInRect(&point, &buttonBox))
			emit();
	}
}*/


/*void Button::render() const {
	SDL_Renderer* renderer = state->getGame()->getRenderer();
	SDL_Texture* sdl_texture = texture->getSDLTexture();

	SDL_Color buttonColor;
	if (isHovered) {
		buttonColor = { 255, 255 , 0 , 255 };
		drawMarker();
	}
	else {
		buttonColor = { 255, 255 , 255 , 255 };
	}

	texture->render(buttonBox, buttonColor);
}*/


^/*void Button::update() {
	setButtonBox(static_cast<int>(position.getX()), static_cast<int>(position.getY()), width, height);
	SDL_Point point;
	SDL_GetMouseState(&point.x, &point.y);

	isHovered = SDL_PointInRect(&point, &buttonBox);
}*/


/*void Button::emit() const
{
	for (OnClickCallback callback : callbacks) {
		callback();
	}
}


void Button::drawMarker() const {
	SDL_Renderer* renderer = state->getGame()->getRenderer();
	SDL_Rect markerRect = { buttonBox.x - 50, buttonBox.y, 30, 30 };
	Texture* markerTexture = state->getGame()->getTexture(Game::MUSHROOM);
	markerTexture->render(markerRect);
}


void Button::setButtonBox(int x, int y, int w, int h) {
	buttonBox = { x, y, w, h };
}*/