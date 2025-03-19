#include "TimerCountdown.h"

TimerCountdown::TimerCountdown(int duration) : totalDuration(std::chrono::seconds(duration)), elapsed(0), timeMultiplier(1.0f), paused(false) {}

void TimerCountdown::start() {
    startTime = std::chrono::steady_clock::now();
}

void TimerCountdown::update() {
    if (!paused) {
        auto now = std::chrono::steady_clock::now();
        getTimeLeft();    // This is an int, can be used to show on UI
        if (isFinished()) {
            // Script to activate boss? Trap? Idk
        }
    }
}

void TimerCountdown::addTime(int seconds) {
    totalDuration += std::chrono::seconds(seconds);
    if (totalDuration.count() < 0) {
        totalDuration = std::chrono::seconds(0); // Prevent negative time
    }
}

void TimerCountdown::setSpeedMultiplier(float multiplier) {
    if (timeMultiplier != multiplier) {
        addTime(-getElapsedTime()/1000);  // Creates new timer starting from last timer point but with different timeMultiplier (If timer was in 3:45, creates a new timer from this point but with different speed)
        timeMultiplier = multiplier;
        start();
    }
}

bool TimerCountdown::isFinished() const {
    return getTimeLeft() == 0;
}

int TimerCountdown::getTimeLeft() const {
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>((now - startTime) * timeMultiplier);
    float timeLeft = std::chrono::duration_cast<std::chrono::seconds>(totalDuration).count() - elapsed.count();
    if (timeLeft < 0) {
        timeLeft = 0;
    }
    return timeLeft;
}

int TimerCountdown::getElapsedTime() const {
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>((now - startTime) * timeMultiplier).count();
    return elapsed;
}

void TimerCountdown::pause() {
    if (paused) return;                           // Do nothing if the timer isn't running
    paused = true;
    pauseTime = std::chrono::steady_clock::now(); // Store the time when paused
}

void TimerCountdown::resume() {
    if (!paused) return;
    auto now = std::chrono::steady_clock::now();
    auto pauseDuration = now - pauseTime;         // Calculate how long it was paused
    startTime += pauseDuration;                   // Adjust startTime according pause duration
    paused = false;
}