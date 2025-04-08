#pragma once

#include "Minigame.h"
#include "../ecs/Component.h"
#include <SDL.h>

// Define chest quality levels
enum class ChestQuality { COMMON, RARE, LEGENDARY };

class MinigameGeneratorComponent : public ecs::Component {
private:
    TimerCountdown* gameTimer;        // Reference to the main game timer
    SDL_Renderer* mainRenderer;       // Reference to the SDL renderer

public:
    __CMPID_DECL__(ecs::cmp::MINIGAMEGENERATORCMP);

    MinigameGeneratorComponent(TimerCountdown* timer, SDL_Renderer* renderer);
    virtual ~MinigameGeneratorComponent();

    // Generates a new minigame based on chest quality
    Minigame* generateMinigame(ChestQuality chestQuality);

private:
    // Sets minigame parameters based on chest quality
    void setMinigameParameters(ChestQuality quality, int& vectorSize, int& holeSize, float& frequency);
};
