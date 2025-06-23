#include "ImageWithFrames.h"

#include "../ecs/Manager.h"
#include "../sdlutils/macros.h"
#include "../sdlutils/Texture.h"
#include "../ecs/Transform.h"
#include "Camera.h"

#include <iostream>
using namespace std;

void ImageWithFrames::initComponent() {
	auto* mngr = _ent->getMngr();
	_tr = mngr->getComponent<Transform>(_ent);
	assert(_tr != nullptr);
}

ImageWithFrames::ImageWithFrames() :
	_tex(nullptr), tBetweenFrames(0), startframe(0), frame(0), cols(1), rows(1), numframes(cols* rows), frameWidth(0), frameHeight(0), tLastFrame(sdlutils().currRealTime()), flip(false) {
}

ImageWithFrames::ImageWithFrames(Texture* tex, int cols, int rows, int frame) :
	_tex(tex), tBetweenFrames(-1), startframe(-1), frame(frame), cols(cols), rows(rows), numframes(cols* rows), tLastFrame(sdlutils().currRealTime()), flip(false) {
	if (_tex) {
		frameWidth = _tex->width() / cols;
		frameHeight = _tex->height() / rows;
	}
}

ImageWithFrames::ImageWithFrames(Texture* tex, float interval, int cols, int rows) :
	_tex(tex), tBetweenFrames(interval), startframe(0), frame(0), cols(cols), rows(rows), numframes(cols* rows), tLastFrame(sdlutils().currRealTime()), flip(false) {
	if (_tex) {
		frameWidth = _tex->width() / cols;
		frameHeight = _tex->height() / rows;
	}
}

ImageWithFrames::ImageWithFrames(Texture* tex, int interval, int cols, int rows, int frameS, int frameN) :
	_tex(tex), tBetweenFrames(interval), startframe(frameS), frame(frameS), cols(cols), rows(rows), numframes(frameN), tLastFrame(sdlutils().currRealTime()), flip(false) {
	if (_tex) {
		frameWidth = _tex->width() / cols;
		frameHeight = _tex->height() / rows;
	}
}

void ImageWithFrames::update() {
	if (sdlutils().currRealTime() - tLastFrame > tBetweenFrames && numframes > 1) {
		if (frame + 1 > startframe + numframes - 1) {
			frame = startframe;
		}
		else {
			frame++;
		}
		tLastFrame = sdlutils().currRealTime();
	}
}

void ImageWithFrames::render() {
	Vector2D screenPos = _tr->getPos() - camOffset;
	SDL_Rect dest = build_sdlrect(screenPos, _tr->getWidth(),
		_tr->getHeight());

	assert(_tex != nullptr);
	if (flip) {
		_tex->render(getRect(), dest, _tr->getRot(), nullptr, SDL_FLIP_HORIZONTAL);
	}
	else {
		_tex->render(getRect(), dest, _tr->getRot(), nullptr, SDL_FLIP_NONE);
	}
}

SDL_Rect ImageWithFrames::getRect() {
	float frameX = ((frame % cols)) * frameWidth;
	float frameY = ((frame / cols)) * frameHeight;
	//cout << frame << "|" << numframes << "   " << cols << "|" << rows << "   " << frame % rows << "|" << frame / cols << "   " << frameX << "|" << frameY << "   " << endl;
	Vector2D framePos = Vector2D{ frameX, frameY };
	SDL_Rect rect = build_sdlrect(framePos, frameWidth, frameHeight);
	return rect;
}