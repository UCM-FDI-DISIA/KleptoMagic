#ifndef TIMER_COUNTDOWN_H  // Check if the identifier is not already defined
#define TIMER_COUNTDOWN_H  // Define it

#include <chrono>

class TimerCountdown {
public:
    TimerCountdown(int duration);   // Constructor to set initial time
    void start();          // Start the timer
    void update();         // Call this in the game loop to track time
    void changeTime(int seconds); // Add or subtract time
    void setSpeedMultiplier(float multiplier); // Control timer acceleration
    bool isFinished() const;  // Check if the countdown is over
    int getTimeLeft() const;  // Get remaining time (For 

private:
    std::chrono::time_point<std::chrono::steady_clock> startTime;
    std::chrono::time_point<std::chrono::steady_clock> eventStartTime;
    float timeMultiplier;
    float multiplierValue;
    bool eventActive;   // Flag to track if event is active
    int duration;       // Total countdown time in seconds
    int elapsed;        // Time already passed
    int eventTime;      // Time passed in acceleration


};

#endif // TIMER_COUNTDOWN_H
