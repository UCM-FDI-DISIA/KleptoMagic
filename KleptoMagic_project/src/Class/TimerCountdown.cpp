#include "TimerCountdown.h"
#include "../ecs/Manager.h"

TimerCountdown::TimerCountdown(int duration) 
    : totalDuration(duration * 1000), timeMultiplier(1.0f), paused(false) {}

void TimerCountdown::update(float deltaTime) {
    if (!paused) {
        addTime(-deltaTime * timeMultiplier);
        if (isFinished()) {
            // Script to activate boss? Trap? Idk
        }
    }
}

//void TimerCountdown::start() {
//    paused = false;
//}

void TimerCountdown::addTime(int milliseconds) {
    totalDuration += milliseconds;
    if (totalDuration < 0) {
        totalDuration = 0; // Prevent negative time
    }
}

void TimerCountdown::setSpeedMultiplier(float multiplier) {
    if (timeMultiplier != multiplier) {
        timeMultiplier = multiplier;
    }
}

bool TimerCountdown::isFinished() const {
    return getTimeLeft() == 0;
}

int TimerCountdown::getTimeLeft() const {
    float timeLeft = totalDuration / 1000;
    if (timeLeft < 0) {
        timeLeft = 0;
    }
    return timeLeft;
}

//void TimerCountdown::pause() {
//    if (paused) return;                           // Do nothing if the timer isn't running
//    paused = true;
//}
//
//void TimerCountdown::resume() {
//    if (!paused) return;
//    paused = false;
//}