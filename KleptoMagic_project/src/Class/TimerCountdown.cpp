#include "TimerCountdown.h"

TimerCountdown::TimerCountdown(int duration) : duration(duration), elapsed(0), eventTime(0), timeMultiplier(1.0f), multiplierValue(1.0f), eventActive(false) {}

void TimerCountdown::start() {
    startTime = std::chrono::steady_clock::now();
}

void TimerCountdown::update() {
    auto now = std::chrono::steady_clock::now();
    float elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime).count();
    if (eventActive) {
        float eventTime = std::chrono::duration_cast<std::chrono::milliseconds>(now - eventStartTime).count();
    }
    else {
        
    }
    elapsed = (elapsed - eventTime) + eventTime * multiplierValue;
}

void TimerCountdown::changeTime(int seconds) {
    duration += seconds;
    if (duration < 0) duration = 0;  // Prevent negative time
}

void TimerCountdown::setSpeedMultiplier(float multiplier) {
    timeMultiplier = multiplier;

    if (!eventActive && multiplier > 1.0f) {
        eventActive = true;
        eventStartTime = std::chrono::steady_clock::now();
        multiplierValue = multiplier;
    }

    if (eventActive && multiplier == 1.0f) {
        eventActive = false;
    }
}

bool TimerCountdown::isFinished() const {
    return elapsed >= duration;
}

int TimerCountdown::getTimeLeft() const {
    int timeLeft = duration - elapsed;
    return (timeLeft < 0) ? 0 : timeLeft;
}

