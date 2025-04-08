#include "Minigame.h"
#include "../sdlutils/NewInputHandler.h"
#include <chrono>
#include <cmath>
#include <SDL.h>

Minigame::Minigame(TimerCountdown& timer, SDL_Renderer* mainGameRenderer, int vectorSize, int holeStart, int holeSize, float frequency) :
					gameTimer(timer), mainRenderer(mainGameRenderer), lockVector(vectorSize, -1 /*Wall*/),
					holeStart(holeStart), holeSize(holeSize), lockpickPosition(0), frequency(frequency),
					elapsedTime(0), running(false), quitMinigame(false), lockpickSpeed(0.1f), waitInterval(2.0f) {}

void Minigame::start() {
	for (int i = 0; i < holeSize; ++i) {
		int index = (holeStart + i) % lockVector.size();					   // If holeStart + holeSize > vectorSize, goes around and continues from the beggining
		lockVector[index] = 1;												   // Hole
	}
	lockpickPosition = 0;
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
		//elapsedTime -= frequency;
		elapsedTime = 0;
		lockpickPosition = (lockpickPosition + 1) % lockVector.size();         // Advance lockpickPosition and loops it back if needed

		if (movingUp) {
			lockpickProgress += lockpickSpeed;
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
			lockpickProgress -= lockpickSpeed;						   // Return to starting position
			if (lockpickProgress <= 0) {
				lockpickProgress = 0;											   // Reset to bottom
			}
		}



#ifdef _DEBUG
		std::cout << "HoleStart: " << holeStart << std::endl;
		std::cout << "Lockpick = " << lockpickPosition << std::endl;
#endif
		render(mainRenderer, lockpickProgress);
	}
}

bool Minigame::attemptPick() {
	if (!running) return false;

	if (lockVector[lockpickPosition] == 1) {
		int penalty = calculatePenalty(lockpickPosition);
		gameTimer.addTime(-penalty);										   // Add the penalty to the main game timer
#ifdef _DEBUG
		std::cout << "Minigame -" << penalty << std::endl;
#endif
		running = false;													   // End the minigame on success
		return true;													       // Successful pick
	}
	else {
		gameTimer.addTime(-10);												   // Hit the wall; subtract 10 seconds and continue
#ifdef _DEBUG
		std::cout << "Minigame -10" << std::endl;
#endif
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

	SDL_SetRenderDrawColor(mainRenderer, 0, 0, 0, 200);                        // Black with 50 alpha to hide main game
	SDL_Rect overlay = { 0, 0, 800, 600 };                                     // Adjust to screen size
	SDL_RenderFillRect(mainRenderer, &overlay);

	SDL_SetRenderDrawColor(mainRenderer, 100, 100, 100, 255);				   // Gray lock
	drawCircle(400, 300, 100, 10);

	SDL_SetRenderDrawColor(mainRenderer, 100, 200, 100, 255);				   // Green hole
	drawHole(400, 300, 100, 10, ((holeStart - lockpickPosition + lockVector.size()) % lockVector.size()), holeSize, lockVector.size());

	SDL_SetRenderDrawColor(mainRenderer, 200, 200, 120, 255);				   // Yellow lockpick
	drawLockpick(400, 300, 100, lockpickProgress);

	SDL_RenderPresent(mainRenderer);										   // Present changes
}

void Minigame::drawCircle(int centerX, int centerY, int radius, int thickness) {
	for (int w = -radius; w <= radius; w++) {
		for (int h = -radius; h <= radius; h++) {
			int dx = w;
			int dy = h;
			int distanceSquared = dx * dx + dy * dy;

			if (distanceSquared >= (radius - thickness) * (radius - thickness) &&
				distanceSquared <= radius * radius) { // Only draw points near the outer edge
				SDL_RenderDrawPoint(mainRenderer, centerX + dx, centerY + dy);
			}
		}
	}
}

void Minigame::drawHole(int centerX, int centerY, int radius, int thickness, float holeStart, float holeSize, int vectorSize) {
	float holeAngle = (static_cast<float>(holeSize) / vectorSize) * 2 * M_PI; // Convert hole size to radians
	float startAngle = (static_cast<float>(holeStart) / vectorSize) * 2 * M_PI; // Convert hole start position to radians
	if (startAngle >= 2 * M_PI) {
		startAngle -= 2 * M_PI;
	}
	float endAngle = startAngle + holeAngle;
#ifdef _DEBUG
	std::cout << startAngle << std::endl;
#endif
	// If hole extends beyond full circle, split it into two segments
	if (endAngle > 2 * M_PI) {
		float overflow = endAngle - 2 * M_PI;

		// First segment (from startAngle to 2PI)
		for (float angle = startAngle; angle < 2 * M_PI; angle += 0.01) {
			int outerX = centerX + radius * std::cos(angle + M_PI / 2);
			int outerY = centerY + radius * std::sin(angle + M_PI / 2);
			int innerX = centerX + (radius - thickness) * std::cos(angle + M_PI / 2);
			int innerY = centerY + (radius - thickness) * std::sin(angle + M_PI / 2);
			SDL_RenderDrawLine(mainRenderer, innerX, innerY, outerX, outerY);
		}

		// Second segment (from 0 to overflow)
		for (float angle = 0; angle < overflow; angle += 0.01) {
			int outerX = centerX + radius * std::cos(angle + M_PI / 2);
			int outerY = centerY + radius * std::sin(angle + M_PI / 2);
			int innerX = centerX + (radius - thickness) * std::cos(angle + M_PI / 2);
			int innerY = centerY + (radius - thickness) * std::sin(angle + M_PI / 2);
			SDL_RenderDrawLine(mainRenderer, innerX, innerY, outerX, outerY);
		}
	}
	else {
		// Normal case (hole is within one continuous section)
		for (float angle = startAngle; angle < endAngle; angle += 0.01) {
			int outerX = centerX + radius * std::cos(angle + M_PI / 2);
			int outerY = centerY + radius * std::sin(angle + M_PI / 2);
			int innerX = centerX + (radius - thickness) * std::cos(angle + M_PI / 2);
			int innerY = centerY + (radius - thickness) * std::sin(angle + M_PI / 2);
			SDL_RenderDrawLine(mainRenderer, innerX, innerY, outerX, outerY);
		}
	}
}

//void Minigame::drawHole(int centerX, int centerY, int radius, int thickness, float holeStart, float holeSize, int vectorSize) {
//	float holeAngle = (static_cast<float>(holeSize) / vectorSize) * 2 * M_PI; // Convert hole size to radians
//	float startAngle = (static_cast<float>(holeStart) / vectorSize) * 2 * M_PI; // Convert hole start position to radians
//
//	// Adjust endAngle for clockwise motion
//	float endAngle = startAngle - holeAngle;
//	if (endAngle < 0) {
//		endAngle += 2 * M_PI; // Wrap around when it goes negative
//	}
//
//	//std::cout << "Start Angle: " << startAngle << ", End Angle: " << endAngle << std::endl;
//
//	// If hole extends beyond 0 radians, split into two parts
//	if (startAngle < holeAngle) {
//		float overflow = holeAngle - startAngle;
//
//		// First segment (from startAngle to 0)
//		for (float angle = startAngle; angle > 0; angle -= 0.01) {
//			int outerX = centerX + radius * std::cos(angle + M_PI / 2);
//			int outerY = centerY + radius * std::sin(angle + M_PI / 2);
//			int innerX = centerX + (radius - thickness) * std::cos(angle + M_PI / 2);
//			int innerY = centerY + (radius - thickness) * std::sin(angle + M_PI / 2);
//			SDL_RenderDrawLine(mainRenderer, innerX, innerY, outerX, outerY);
//		}
//
//		// Second segment (from 2PI to endAngle)
//		for (float angle = 2 * M_PI; angle > endAngle; angle -= 0.01) {
//			int outerX = centerX + radius * std::cos(angle + M_PI / 2);
//			int outerY = centerY + radius * std::sin(angle + M_PI / 2);
//			int innerX = centerX + (radius - thickness) * std::cos(angle + M_PI / 2);
//			int innerY = centerY + (radius - thickness) * std::sin(angle + M_PI / 2);
//			SDL_RenderDrawLine(mainRenderer, innerX, innerY, outerX, outerY);
//		}
//	}
//	else {
//		// Normal case (single continuous section)
//		for (float angle = startAngle; angle > endAngle; angle -= 0.01) {
//			int outerX = centerX + radius * std::cos(angle + M_PI / 2);
//			int outerY = centerY + radius * std::sin(angle + M_PI / 2);
//			int innerX = centerX + (radius - thickness) * std::cos(angle + M_PI / 2);
//			int innerY = centerY + (radius - thickness) * std::sin(angle + M_PI / 2);
//			SDL_RenderDrawLine(mainRenderer, innerX, innerY, outerX, outerY);
//		}
//	}
//}




void Minigame::drawLockpick(int centerX, int centerY, int radius, float progress) {
	int lockpickLength = 80;
	int lockpickWidth = 10;

	int startX = centerX - lockpickWidth / 2;  // Define left edge
	int startY = centerY + radius + 70; // Positioned below the lock

	int endY = startY - std::max(5, static_cast<int>(lockpickLength * progress));

	// Define lockpick rectangle
	SDL_Rect lockpickRect = { startX, endY, lockpickWidth, startY - endY };

	// Render filled rectangle for solid shape
	SDL_RenderFillRect(mainRenderer, &lockpickRect);
}
