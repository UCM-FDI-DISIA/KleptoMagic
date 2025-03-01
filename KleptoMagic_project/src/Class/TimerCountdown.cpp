#include "TimerCountdown.h"

Timer::Timer(int duration) : duration(duration), elapsed(0) {}

void Timer::start() {
    startTime = std::chrono::steady_clock::now();
}

void Timer::update() {
    auto now = std::chrono::steady_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - startTime).count();
}

void Timer::changeTime(int seconds) {
    duration += seconds;
    if (duration < 0) duration = 0;  // Prevent negative time
}

bool Timer::isFinished() const {
    return elapsed >= duration;
}

int Timer::getTimeLeft() const {
    int timeLeft = duration - elapsed;
    return (timeLeft < 0) ? 0 : timeLeft;
}

