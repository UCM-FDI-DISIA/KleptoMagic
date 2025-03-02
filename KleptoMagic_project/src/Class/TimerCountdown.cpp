#include "TimerCountdown.h"

TimerCountdown::TimerCountdown(int duration) : totalDuration(std::chrono::seconds(duration)), elapsed(0), timeMultiplier(1.0f), multiplierValue(1.0f), eventActive(false), running(false) {}

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

    if (!eventActive && multiplier > 1.0f) {
        eventActive = true;
        multiplierValue = multiplier;
    }

    if (eventActive && multiplier == 1.0f) {
        eventActive = false;
    }
}

bool TimerCountdown::isFinished() const {
    return !running;
}

int TimerCountdown::getTimeLeft() const {
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>((now - startTime) * timeMultiplier);
    int timeLeft = std::chrono::duration_cast<std::chrono::seconds>(totalDuration).count() - elapsed.count();
    return (timeLeft < 0) ? 0 : timeLeft;
}

