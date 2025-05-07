#pragma once

#include "../room/TimerCountdown.h"
#include "../ecs/Component.h"
#include <SDL.h>

class TimerRenderer : public ecs::Component {
private:
    TimerCountdown* gameTimer;        // Reference to the main game timer
    SDL_Renderer* mainRenderer;       // Reference to the SDL renderer

public:
    __CMPID_DECL__(ecs::cmp::TIMERRENDERERCMP);

    TimerRenderer(TimerCountdown* timer, SDL_Renderer* renderer);
    virtual ~TimerRenderer();

    void render(SDL_Renderer* mainGameRenderer, int timeLeft); // Visual renderization

private:

};

