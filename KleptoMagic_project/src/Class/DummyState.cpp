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
    }

    std::cout << "Time's up!" << std::endl;
}