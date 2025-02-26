#pragma once
#ifndef BUTTON_H
#define BUTTON_H

#include "GameObject.h"
#include <vector>
using OnClickCallback = std::function<void()>;

class Button : public GameObject {
private:
	std::vector<OnClickCallback> callbacks;
	//SDL_Rect buttonBox;
	//bool isHovered;
	//int width, height;

public:
	//Button(GameState* state, Texture* tex, Point2D pos, int w, int h);
	//GameState* getState() const { return state; };
	//void setButtonBox(int x, int y, int w, int h);
	void addCallback(OnClickCallback callback);
	//void handleEvent(const SDL_Event& event) override;
	void render() const override;
	void update() override;
	//void emit() const;
	//void drawMarker() const;
};

#endif // BUTTON_H