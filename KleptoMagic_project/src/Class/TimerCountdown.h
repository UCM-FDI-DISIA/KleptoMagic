#ifndef TIMERCOUNTDOWN_H
#define TIMERCOUNTDOWN_H

#include <chrono>
#include "..\ecs\Component.h"

class TimerCountdown : public ecs::Component {
public:
    TimerCountdown(int duration);                                   // Constructor to set initial time
    void start();                                                   // Start the timer
    void update();                                                  // Call this in the game loop to track time
    void addTime(int seconds);                                      // Add or subtract time (Negative numbers to substract)
    void setSpeedMultiplier(float multiplier);                      // Control timer acceleration
    bool isFinished() const;                                        // Check if the countdown is over
    int getTimeLeft() const;                                        // Get remaining time (For visual purpose)
    int getElapsedTime() const;                                     // Used to update timer whenever setSpeedMultiplier is used
    void pause(); 
    void resume();

private:
    std::chrono::time_point<std::chrono::steady_clock> startTime;
    std::chrono::duration<float> totalDuration;
    std::chrono::time_point<std::chrono::steady_clock> pauseTime;
    float timeMultiplier;
    bool paused;
    int elapsed;                                                    // Time already passed
};

#endif // TIMERCOUNTDOWN_H
