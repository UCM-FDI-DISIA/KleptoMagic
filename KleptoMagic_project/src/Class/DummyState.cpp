#include "DummyState.h"
#include <iostream>

DummyState::DummyState() : GameState(game), gameTimer(300) {

    
    
    //Al final de todo
    gameTimer.start();
}

void DummyState::update() {
    timerTest();
}

void DummyState::timerTest() {
    // Game loop (this would run every frame)
    while (!gameTimer.isFinished()) {
        gameTimer.update(); // Update the timer (ticks down based on real time)
#ifdef _DEBUG
        std::cout << "Time left: " << gameTimer.getTimeLeft() << " seconds\n";
#endif
        // This is only for testing purposes. Use the respective methods to apply them properly (eventActive = True/False & gameTimer.addTime(_myTime))
        if (gameTimer.getTimeLeft() < 298) {
            if (gameTimer.getTimeLeft() < 290) {
                if (gameTimer.getTimeLeft() < 248) {
                    if (gameTimer.getTimeLeft() < 220) {
                        gameTimer.setSpeedMultiplier(1.0f);
                    }
                    else { gameTimer.setSpeedMultiplier(4.0f); }
                }
                else { gameTimer.setSpeedMultiplier(1.0f); }
            }
            else { gameTimer.setSpeedMultiplier(2.0f); }
        }
        if (gameTimer.getTimeLeft() < 288 && gameTimer.getTimeLeft() > 250) {
            gameTimer.addTime(-38);
        }
        if (gameTimer.getTimeLeft() < 215 && gameTimer.getTimeLeft() > 200) {
            gameTimer.addTime(-999);
        }
    }
#ifdef _DEBUG
    std::cout << "Time's up!" << std::endl;
#endif
    
}