#pragma once
#ifndef BUTTON_H
#define BUTTON_H

#include "GameObject.h"
#include <functional>
#include "../utils/Vector2D.h"
#include <vector>
#include "GameState.h"
#include "Game.h" 
using OnClickCallback = std::function<void()>;

class Button : public GameObject, public EventHandler {
private:
	std::vector<OnClickCallback> callbacks;
	SDL_Rect buttonBox;
	Vector2D position;
	bool isHovered;
	int width, height;

public:
	Button(GameState* state, Texture* tex, Vector2D pos, int w, int h);
	GameState* getState() const { return state; };
	void setButtonBox(int x, int y, int w, int h);
	void addCallback(OnClickCallback callback);
	void handleEvent(const SDL_Event& event);
	void render() const override;
	void update() override;
	void emit() const;
};

#endif // BUTTON_H