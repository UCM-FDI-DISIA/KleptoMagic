#include "Minigame.h"
#include <chrono>
#include <cmath>

Minigame::Minigame(TimerCountdown& timer, int vectorSize, int holeStart, int holeSize, float frequency) :
					gameTimer(timer), lockVector(vectorSize, -1 /*Wall*/), holeStart(holeStart), holeSize(holeSize), lockpickPosition(0), frequency(frequency), elapsedTime(0), running(false) {}

void Minigame::start() {
	for (int i = 0; i < holeSize; ++i) {
		int index = (holeStart + i) % lockVector.size();					   // If holeStart + holeSize > vectorSize, goes around and continues from the beggining
		lockVector[index] = 1;												   // Hole
	}
	elapsedTime = 0;														   // To calculate frequency
	running = true;

	gameTimer.setSpeedMultiplier(2.0);
}

void Minigame::end() {
	if (!quitMinigame) {
		// Script for rewards
	}
	gameTimer.setSpeedMultiplier(1.0);
	// Script for changing scenes
}

void Minigame::minigameLogic(float deltaTime) {
	if (!running) {
		end();
	}
	
	if (/*InputManager "Left Click" (Maybe?)*/) {
		attemptPick();
	}

	if (/*InputManager "Esc" (Maybe?)*/) {
		quitMinigame = true;
		running = false;													   // End Minigame
	}	

	elapsedTime += deltaTime;

	while (elapsedTime >= frequency) {
		elapsedTime -= frequency;
		lockpickPosition = (lockpickPosition + 1) % lockVector.size();         // Advance lockpickPosition and loops it back if needed
	}

}

bool Minigame::attemptPick() {
	if (!running) return false;

	if (lockVector[lockpickPosition] == 1) {
		int penalty = calculatePenalty(lockpickPosition);
		gameTimer.addTime(-penalty);										   // Add the penalty to the main game timer
		running = false;													   // End the minigame on success
		return true;													       // Successful pick
	}
	else {
		gameTimer.addTime(-10);												   // Hit the wall; subtract 10 seconds and continue
		return false;														   // Failed attempt
	}
}

int Minigame::calculatePenalty(int position) {
	int holeCenter = (holeStart + holeSize / 2) % lockVector.size();
	int distanceFromCenter = std::min(										   // Takes the shortest distance to the center of the hole
		std::abs(position - holeCenter),                                       // Direct distance
		static_cast<int>(lockVector.size()) - std::abs(position - holeCenter)  // Distance from the edges (In the case lockpickPosition is at 5 and holeCenter at 195 (VectorSize 200) for example)
	);
	int maxDistance = holeSize / 2;
	
	return 9 * distanceFromCenter / maxDistance;                               // Calculates penalty time < 10, being 0 if on the exact center
}