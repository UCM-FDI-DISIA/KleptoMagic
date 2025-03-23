#pragma once
#ifndef BUTTON_H
#define BUTTON_H

#include "../ecs/ecs.h"
#include "../ecs/ecs_defs.h"
#include "../ecs/Component.h"
#include "EventHandler.h"
#include "../sdlutils/InputHandler.h"
#include "InputManager.h"
#include "../utils/Vector2D.h"
#include "../sdlutils/Texture.h"

#include <functional>
#include <SDL.h>

using OnClickCallback = std::function<void()>;

class Button : public ecs::Component, public EventHandler {
public:
    __CMPID_DECL__(ecs::cmp::BUTTON)

        Button(std::function<void()> onClick, Vector2D position, Vector2D size, Texture* texture);
    void initComponent() override;
    void update() override;
    void render() override;
    void handleEvent(const SDL_Event& event) override;

private:
    bool isInside(int x, int y) const;
    std::function<void()> _onClick;
    Vector2D _position;
    Vector2D _size;
    InputHandler* _inputHandler;
    Texture* _texture; 
};

#endif // BUTTON_H

//#pragma once
//#ifndef BUTTON_H
//#define BUTTON_H
//
//#include "GameObject.h"
//#include <functional>
//#include "../utils/Vector2D.h"
//#include <vector>
////#include "GameState.h"
//#include "Game.h" 
//using OnClickCallback = std::function<void()>;
//
//class Button{
//private:
//	std::vector<OnClickCallback> callbacks;
//	SDL_Rect buttonBox;
//	Vector2D position;
//	bool isHovered;
//	int width, height;
//
//public:
//	Button(GameState* state, Texture* tex, Vector2D pos, int w, int h);
//	//GameState* getState() const { return state; };
//	void setButtonBox(int x, int y, int w, int h);
//	void addCallback(OnClickCallback callback);
//	void handleEvent(const SDL_Event& event);
//	//void render() const override;
//	//void update() override;
//	void emit() const;
//};
//
//#endif // BUTTON_H
