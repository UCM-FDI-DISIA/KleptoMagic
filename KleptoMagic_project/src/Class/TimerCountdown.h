#ifndef TIMERCOUNTDOWN_H
#define TIMERCOUNTDOWN_H

#include <chrono>
#include "../sdlutils/SDLUtils.h"
#include "../ecs/Component.h"

class TimerCountdown : public ecs::Component {
public:
    TimerCountdown(int duration);                                   // Constructor to set initial time
    //void start();                                                 // Start the timer
    void update(float deltaTime);                                   // Call this in the game loop to track time
    void addTime(int milliseconds);                                 // Add or subtract time in milliseconds (Negative numbers to substract)
    void setSpeedMultiplier(float multiplier);                      // Control timer acceleration
    bool isFinished() const;                                        // Check if the countdown is over
    int getTimeLeft() const;                                        // Get remaining time (For visual purpose)
    //void pause(); 
    //void resume();
    bool paused;

private:
    float totalDuration;
    float timeMultiplier;
};

#endif // TIMERCOUNTDOWN_H
