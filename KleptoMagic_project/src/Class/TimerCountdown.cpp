#include "TimerCountdown.h"

TimerCountdown::TimerCountdown(int duration) : totalDuration(std::chrono::seconds(duration)), elapsed(0), timeMultiplier(1.0f), running(false) {}

void TimerCountdown::start() {
    startTime = std::chrono::steady_clock::now();
    running = true;
}

void TimerCountdown::update() {
    auto now = std::chrono::steady_clock::now();
    float elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - startTime).count();
}

void TimerCountdown::addTime(int seconds) {
    totalDuration += std::chrono::seconds(seconds);
    if (totalDuration.count() < 0) {
        totalDuration = std::chrono::seconds(0); // Prevent negative time
    }
}

void TimerCountdown::setSpeedMultiplier(float multiplier) {
    timeMultiplier = multiplier;
}

bool TimerCountdown::isFinished() const {
    return !running;
}

int TimerCountdown::getTimeLeft() const {
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>((now - startTime) * timeMultiplier);
    float timeLeft = std::chrono::duration_cast<std::chrono::seconds>(totalDuration).count() - elapsed.count();
    return (timeLeft < 0) ? 0 : timeLeft;
}

int TimerCountdown::getElapsedTime() const {
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>((now - startTime) * timeMultiplier).count();
    return static_cast<int>(elapsed);
}

