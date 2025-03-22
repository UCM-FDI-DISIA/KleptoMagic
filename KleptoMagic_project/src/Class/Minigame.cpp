#include "Minigame.h"
#include "../sdlutils/NewInputHandler.h"
#include <chrono>
#include <cmath>
#include <SDL.h>

Minigame::Minigame(TimerCountdown& timer, SDL_Renderer* mainGameRenderer, int vectorSize, int holeStart, int holeSize, float frequency) :
					gameTimer(timer), mainRenderer(mainGameRenderer), lockVector(vectorSize, -1 /*Wall*/),
					holeStart(holeStart), holeSize(holeSize), lockpickPosition(0), frequency(frequency),
					elapsedTime(0), running(false), quitMinigame(false), lockpickSpeed(0.5f), waitInterval(1.0f) {}

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


}

void Minigame::minigameLogic(float deltaTime) {
	if (!running) {
		end();
	}
	
	static float lockpickProgress = 0;										   // Lockpick movement progress (0: bottom, 1: fully extended)
	static bool movingUp = false;											   // Whether the lockpick is moving up
	static bool waiting = false;											   // Whether we are waiting between tries
	static float waitTimeElapsed = 0;										   // Timer for waiting interval

	// Check if we're in a waiting period
	if (waiting) {
		waitTimeElapsed += deltaTime;
		if (waitTimeElapsed >= waitInterval) {
			waiting = false;
			waitTimeElapsed = 0;
		}
	}

	// Handle lockpick movement logic
	if (movingUp) {
		lockpickProgress += lockpickSpeed * deltaTime;
		if (lockpickProgress >= 1.0f) {
			lockpickProgress = 1.0f;
			movingUp = false;

			if (attemptPick()) {											   // Successful attempt
				
			}
			else {															   // Failed attempt
				waiting = true;												   // Start cooldown
			}
		}
	}
	else if (lockpickProgress > 0) {
		lockpickProgress -= lockpickSpeed * deltaTime;						   // Return to starting position
		if (lockpickProgress <= 0) {
			lockpickProgress = 0;											   // Reset to bottom
		}
	}

	if (!waiting && !movingUp) {
		if (NewInputHandler::Instance()->isActionPressed(Action::SHOOT)) {
			movingUp = true;
		}
	}

	if (NewInputHandler::Instance()->isActionPressed(Action::QUIT)) {
		quitMinigame = true;
		running = false;													   // End Minigame
	}	

	elapsedTime += deltaTime;

	while (elapsedTime >= frequency) {
		elapsedTime -= frequency;
		lockpickPosition = (lockpickPosition + 1) % lockVector.size();         // Advance lockpickPosition and loops it back if needed
	}

	render(mainRenderer, lockpickProgress);
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

void Minigame::render(SDL_Renderer* mainGameRenderer, float lockpickProgress) {
	if (!running) return;

	SDL_SetRenderDrawColor(mainRenderer, 0, 0, 0, 150);                        // Black with 150 alpha to hide main game
	SDL_Rect overlay = { 0, 0, 800, 600 };                                     // Adjust to screen size
	SDL_RenderFillRect(mainRenderer, &overlay);

	SDL_SetRenderDrawColor(mainRenderer, 100, 100, 100, 255);				   // Gray lock
	drawCircle(400, 300, 100);

	SDL_SetRenderDrawColor(mainRenderer, 100, 200, 100, 255);				   // Green hole
	drawHole(400, 300, 100, lockpickPosition, holeSize);

	SDL_SetRenderDrawColor(mainRenderer, 200, 200, 120, 255);				   // Yellow lockpick
	drawLockpick(400, 300, 100, lockpickProgress);

	SDL_RenderPresent(mainRenderer);										   // Present changes
}

void Minigame::drawCircle(int centerX, int centerY, int radius) {
	for (int w = 0; w < radius * 2; w++) {
		for (int h = 0; h < radius * 2; h++) {
			int dx = radius - w;
			int dy = radius - h;
			if ((dx * dx + dy * dy) <= (radius * radius)) {					   // Checks if the current point is inside the circle
				SDL_RenderDrawPoint(mainRenderer, centerX + dx, centerY + dy);
			}
		}
	}
}

void Minigame::drawHole(int centerX, int centerY, int radius, float startAngle, float holeSize) {
	float endAngle = startAngle + holeSize;
	for (float angle = startAngle; angle < endAngle; angle += 0.01) {		   // Goes through the hole's angle range
		int x = centerX + radius * std::cos(angle);
		int y = centerY + radius * std::sin(angle);
		SDL_RenderDrawPoint(mainRenderer, x, y);
	}
}

void Minigame::drawLockpick(int centerX, int centerY, int radius, float progress) {
	int lockpickLength = 50; // The length of the lockpick stick

	// Calculate the lockpick's current endpoint
	int startX = centerX;
	int startY = centerY + radius + 10; // Start slightly below the circle
	int endX = centerX;
	int endY = startY - static_cast<int>(lockpickLength * progress); // Move upward based on progress

	// Draw the lockpick
	SDL_RenderDrawLine(mainRenderer, startX, startY, endX, endY);
}