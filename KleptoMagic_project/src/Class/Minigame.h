#pragma once

#include "TimerCountdown.h"
#include <vector>

class Minigame {
public:

    Minigame(TimerCountdown& timer, int vectorSize, int holeStart, int holeSize, float frequency); // Constructor (Introduce main game timer here)
	void start();                        // Sets Timer's speed multiplier
	void end();                          // Resets Timer's speed multiplier (Should also switch scenes)
	void minigameLogic(float deltaTime); // Runs minigame
    bool attemptPick();                  // Player attempts to stop the lockpick

private:
	TimerCountdown& gameTimer;
    std::vector<int> lockVector;         // The circle with walls and the hole     (Vector Size should be in between [100-300])
    int holeStart;                       // Starting position of the hole          ([Random] in between Vector Size)
    int holeSize;                        //                                        (Should be in between [15-25]% of Vector Size)
    int lockpickPosition;                // Current position of the lockpick
    float frequency;                     // Speed of the lockpick's movement       (Should be [0.01] positions/sec)
    float elapsedTime;
    bool running;                        // If the minigame is currently active

    int calculatePenalty(int position);  // Calculates penalty based on the lockpicks's position in the hole
};