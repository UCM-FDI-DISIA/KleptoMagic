#include <chrono>

class Timer {
public:
    Timer(int duration);   // Constructor to set initial time
    void start();          // Start the timer
    void update();         // Call this in the game loop to track time
    void addTime(int seconds);   // Increase time
    void subtractTime(int seconds); // Decrease time
    bool isFinished() const;  // Check if the countdown is over
    int getTimeLeft() const;  // Get remaining time

private:
    std::chrono::time_point<std::chrono::steady_clock> startTime;
    int duration;  // Total countdown time in seconds
    int elapsed;   // Time already passed
};
