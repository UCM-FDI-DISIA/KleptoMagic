#pragma once
#include "../ecs/Component.h"
#include "../sdlutils/Texture.h"
#include "../sdlutils/SDLUtils.h"

class Texture;

struct ImageWithFrames : public ecs::Component {

	ImageWithFrames() :
		_tex(nullptr), tBetweenFrames(0), startframe(0), frame(0), cols(1), rows(1), numframes(cols* rows), frameWidth(0), frameHeight(0), tLastFrame(sdlutils().currRealTime()) {
	}

	ImageWithFrames(Texture* tex, int cols, int rows, int frame) :
		_tex(tex), tBetweenFrames(-1), startframe(-1), frame(frame), cols(cols), rows(rows), numframes(cols* rows), tLastFrame(sdlutils().currRealTime()) {
		if (_tex) {
			frameWidth = _tex->width() / cols;
			frameHeight = _tex->height() / rows;
		}
	}

	ImageWithFrames(Texture* tex, float interval, int cols, int rows) :
		_tex(tex), tBetweenFrames(interval), startframe(0), frame(0), cols(cols), rows(rows), numframes(cols * rows), tLastFrame(sdlutils().currRealTime()) {
		if (_tex) {
			frameWidth = _tex->width() / cols;
			frameHeight = _tex->height() / rows;
		}
	}

	ImageWithFrames(Texture* tex, int interval, int cols, int rows, int frameS, int frameN) :
		_tex(tex), tBetweenFrames(interval), startframe(frameS), frame(frameS), cols(cols), rows(rows), numframes(frameN), tLastFrame(sdlutils().currRealTime()) {
		if (_tex) {
			frameWidth = _tex->width() / cols;
			frameHeight = _tex->height() / rows;
		}
	}

	virtual ~ImageWithFrames() {
	}

	void update() {
		if (sdlutils().currRealTime() - tLastFrame > tBetweenFrames && startframe != -1) {
			if (frame + 1 > startframe + numframes - 1) {
				frame = startframe;
			}
			else {
				frame++;
			}
			tLastFrame = sdlutils().currRealTime();
		}
	}

	SDL_Rect getRect() {
		float frameX = ((frame % cols)) * frameWidth;
		float frameY = ((frame / rows)) * frameHeight;
		Vector2D framePos = Vector2D{ frameX, frameY };
		SDL_Rect rect = build_sdlrect(framePos, frameWidth, frameHeight);
		return rect;
	}

	Texture* _tex;
	int tBetweenFrames;
	int tLastFrame;
	int frame;
	int cols;
	int rows;
	int frameWidth;
	int frameHeight;
	int startframe;
	int numframes;
};
