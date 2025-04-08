#include "ImageWithFrames.h"

#include <cassert>

#include "../ecs/Manager.h"
#include "../sdlutils/macros.h"
#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/Texture.h"
#include "Transform.h"

ImageWithFrames::ImageWithFrames() :
	_tr(), _tex() {
}

ImageWithFrames::ImageWithFrames(Texture* tex, int cols, int rows) :
	_tr(), _tex(tex),
	cols_(cols), rows_(rows),  // Inicializar cols_ y rows_
	totalFrames_(cols* rows),
	frameWidth_(_tex->width() / cols),  // Usar cols, no frameWidth_
	frameHeight_(_tex->height() / rows),
	currentFrame_(0),
	currentTime_(0) {
	frameTime_ = 50.0f;
}

ImageWithFrames::~ImageWithFrames() {
}

void ImageWithFrames::initComponent() {
	auto mngr = _ent->getMngr();
	_tr = mngr->getComponent<Transform>(_ent);
	assert(_tr != nullptr);
}

void ImageWithFrames::render() {
	// Calcular posición del frame en la spritesheet
	int row = currentFrame_ / (cols_);
	int col = currentFrame_ % (cols_);

	SDL_Rect srcRect = {
		col * frameWidth_,
		row * frameHeight_,
		frameWidth_,
		frameHeight_
	};

	SDL_Rect destRect = build_sdlrect(_tr->getPos(), _tr->getWidth(), _tr->getHeight());

	// Renderizar el frame específico
	_tex->render(srcRect, destRect, _tr->getRot());
}

void ImageWithFrames::update() {
	// Actualizar el tiempo utilizando virtualTimer
	currentTime_ += sdlutils().virtualTimer().currRealTime();


	// Cambiar frame si se supera el tiempo
	if (currentTime_ >= frameTime_) {
		currentFrame_ = (currentFrame_ + 1) % totalFrames_;
		sdlutils().virtualTimer().resetTime();
		currentTime_ = 0;
	}
}
