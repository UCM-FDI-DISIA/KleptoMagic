#include "DummyState.h"
#include "TimerCountdown.h"
#include <iostream>

DummyState::DummyState() : GameState(game), gameTimer(300), eventActive(false) {
   
}

void DummyState::update() {
    gameTimer.start();
    bool lastEventActive = false;

    // Game loop (this would run every frame)
    while (!gameTimer.isFinished()) {
        gameTimer.update(); // Update the timer (ticks down based on real time)
        std::cout << "Time left: " << gameTimer.getTimeLeft() << " seconds\n";
        
        // This is only for testing purposes. Use the respective methods to apply them properly (eventActive = True/False & gameTimer.addTime(_myTime))
        if (gameTimer.getTimeLeft() < 290) {
            if (gameTimer.getTimeLeft() < 270) {
                eventActive = false;
            }
            else { eventActive = true; }
        }
        if (gameTimer.getTimeLeft() < 240) {
            if (gameTimer.getTimeLeft() < 220) {
                eventActive = false;
            }
            else { eventActive = true; }
        }
        if (gameTimer.getTimeLeft() < 260 && gameTimer.getTimeLeft() > 250) {
            gameTimer.addTime(-15);
        }

        // If event switches active state...
        if (eventActive && !lastEventActive) {
            gameTimer.setSpeedMultiplier(2.0f);
        }
        else if (lastEventActive && !eventActive) {
            gameTimer.setSpeedMultiplier(1.0f);
        }
        lastEventActive = eventActive;
    }

    std::cout << "Time's up!" << std::endl;
}