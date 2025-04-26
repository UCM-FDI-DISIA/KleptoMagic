#pragma once
#include "../ecs/Component.h"
#include "../sdlutils/SDLUtils.h"
#include "Transform.h"

class Transform;
class Texture;

class ImageWithFrames : public ecs::Component {

public:
	__CMPID_DECL__(ecs::cmp::IMAGEWITHFRAMES)

	void initComponent() override;

	ImageWithFrames();
	ImageWithFrames(Texture* tex, int cols, int rows, int frame);
	ImageWithFrames(Texture* tex, float interval, int cols, int rows);
	ImageWithFrames(Texture* tex, int interval, int cols, int rows, int frameS, int frameN);

	virtual ~ImageWithFrames() {};

	void update();
	void render();

	SDL_Rect getRect();

	void setFrame(int f) { frame = f; };
	void setStartingFrame(int f) { startframe = f; tLastFrame = sdlutils().currRealTime(); };
	void setInterval(int i) { tBetweenFrames = i; tLastFrame = sdlutils().currRealTime(); };
	void setNumFrames(int n) { numframes = n; tLastFrame = sdlutils().currRealTime();};
	void setFlip(bool f) { flip = f; };

private:
	Texture* _tex;
	Transform* _tr;
	int tBetweenFrames;
	int tLastFrame;
	int frame;
	int cols;
	int rows;
	int frameWidth;
	int frameHeight;
	int startframe;
	int numframes;
	bool flip;
};
