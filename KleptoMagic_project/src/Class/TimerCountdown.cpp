#include "TimerCountdown.h"

TimerCountdown::TimerCountdown(int duration) : totalDuration(std::chrono::seconds(duration)), elapsed(0), timeMultiplier(1.0f), paused(false) {}

void TimerCountdown::start() {
    startTime = std::chrono::steady_clock::now();
}

void TimerCountdown::update() {
    if (!paused) {
        auto now = std::chrono::steady_clock::now();
        float elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - startTime).count();
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
        addTime(-getElapsedTime());  // Creates new timer starting from last timer point but with different timeMultiplier
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
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>((now - startTime) * timeMultiplier).count();
    return elapsed;
}

void TimerCountdown::pause() {
    if (paused) return; // Do nothing if the timer isn't running
    addTime(-getElapsedTime()); // Subtract elapsed time from total duration
    paused = true;
}

void TimerCountdown::resume() {
    if (!paused) return;
    start();             // Restart the timer
    paused = false;
}