#include "MinigameGeneratorComponent.h"
#include <cstdlib>

MinigameGeneratorComponent::MinigameGeneratorComponent(TimerCountdown* timer, SDL_Renderer* renderer)
    : gameTimer(timer), mainRenderer(renderer) {}

MinigameGeneratorComponent::~MinigameGeneratorComponent() {}

Minigame* MinigameGeneratorComponent::generateMinigame(ChestQuality chestQuality) {
    int vectorSize = 0;
    int holeSize = 0;
    float frequency = 0.0f;

    setMinigameParameters(chestQuality, vectorSize, holeSize, frequency);

    int holeStart = rand() % vectorSize;

    return new Minigame(*gameTimer, mainRenderer, vectorSize, holeStart, holeSize, frequency);
}

void MinigameGeneratorComponent::setMinigameParameters(ChestQuality quality, int& vectorSize, int& holeSize, float& frequency) {
    switch (quality) {
    case ChestQuality::COMMON:
        vectorSize = 600 + (rand() % 301);                 // Random between 600 and 900
        frequency = 2.0f;
        holeSize = static_cast<int>(vectorSize * 0.25f);   // 25% of vector size
        break;

    case ChestQuality::RARE:
        vectorSize = 200 + (rand() % 101);                 // Random between 200 and 300
        frequency = 1.5f;
        holeSize = static_cast<int>(vectorSize * 0.20f);   // 20% of vector size
        break;

    case ChestQuality::LEGENDARY:
        vectorSize = 150 + (rand() % 101);                 // Random between 150 and 250
        frequency = 1.0f;
        holeSize = static_cast<int>(vectorSize * 0.15f);   // 15% of vector size
        break;

    default:
        vectorSize = 250;                                  // Default vector size
        frequency = 2.0f;                                  // Default frequency
        holeSize = static_cast<int>(vectorSize * 0.20f);   // Default hole size (20%)
        break;
    }
}
