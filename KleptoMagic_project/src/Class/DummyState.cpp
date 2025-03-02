#include "DummyState.h"
#include "TimerCountdown.h"
#include <iostream>

DummyState::DummyState() : GameState(game), gameTimer(300), eventActive(false) {

}

void DummyState::update() {
    gameTimer.start();

    // Game loop (this would run every frame)
    while (!gameTimer.isFinished()) {
        gameTimer.update(); // Update the timer (ticks down based on real time)
        std::cout << "Time left: " << gameTimer.getTimeLeft() << " seconds\n";

        // Other game logic, like checking for events
        if (eventActive) {
            gameTimer.setSpeedMultiplier(2.0f);  // Double speed when event is active
        }
        else {
            gameTimer.setSpeedMultiplier(1.0f);  // Normal speed when event ends
        }
    }

    std::cout << "Time's up!" << std::endl;
}