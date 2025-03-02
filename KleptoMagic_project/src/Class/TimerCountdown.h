#ifndef TIMER_COUNTDOWN_H
#define TIMER_COUNTDOWN_H

#include <chrono>

class TimerCountdown {
public:
    TimerCountdown(int duration);   // Constructor to set initial time
    void start();          // Start the timer
    void update();         // Call this in the game loop to track time
    void addTime(int seconds); // Add or subtract time
    void setSpeedMultiplier(float multiplier); // Control timer acceleration
    bool isFinished() const;  // Check if the countdown is over
    int getTimeLeft() const;  // Get remaining time (For visual purpose)

private:
    std::chrono::time_point<std::chrono::steady_clock> startTime;
    std::chrono::duration<float> totalDuration;
    float timeMultiplier;
    float multiplierValue;
    bool eventActive;   // Flag to track if event is active
    bool running;       // Timer ongoing
    int elapsed;        // Time already passed
};

#endif // TIMER_COUNTDOWN_H
