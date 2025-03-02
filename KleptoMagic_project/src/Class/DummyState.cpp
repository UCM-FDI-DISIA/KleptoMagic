#include "DummyState.h"
#include <iostream>

DummyState::DummyState() : GameState(game), gameTimer(300), eventActive(false) {

    
    
    //Al final de todo
    gameTimer.start();
}

void DummyState::update() {
    //timerTest();
}

void DummyState::timerTest() {
    // Game loop (this would run every frame)
    while (!gameTimer.isFinished()) {
        gameTimer.update(); // Update the timer (ticks down based on real time)
#ifdef _DEBUG
        std::cout << "Time left: " << gameTimer.getTimeLeft() << " seconds\n";
#endif
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
#ifdef _DEBUG
    std::cout << "Time's up!" << std::endl;
#endif
    
}