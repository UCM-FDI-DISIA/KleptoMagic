    #include "TimerRenderer.h"
    #include <SDL_ttf.h>

    TimerRenderer::TimerRenderer(TimerCountdown* timer, SDL_Renderer* renderer)
        : gameTimer(timer), mainRenderer(renderer) {}

    TimerRenderer::~TimerRenderer() {}

    void TimerRenderer::render(SDL_Renderer* mainGameRenderer, int timeLeft) {
        static SDL_Texture* textTexture = nullptr;  // Static ensures persistence across calls
        static TTF_Font* font;

        // Cleanup font at the end
        TTF_CloseFont(font);

        font = TTF_OpenFont("resources/fonts/arial.ttf", 24);
        if (!font) {
            SDL_Log("Failed to load font: %s", TTF_GetError());
            return;
        }

        std::string timeString = (timeLeft / 60 < 10 ? "0" : "") + std::to_string(timeLeft / 60) + ":" +
                                 (timeLeft % 60 < 10 ? "0" : "") + std::to_string(timeLeft % 60);

        // Create text surface
        SDL_Color white = { 255, 255, 255, 255 };   
        SDL_Surface* textSurface = TTF_RenderText_Solid(font, timeString.c_str(), white);
        if (!textSurface) {
            SDL_Log("Failed to create text surface: %s", TTF_GetError());
            TTF_CloseFont(font);
            return;
        }

        // Create texture from surface
        textTexture = SDL_CreateTextureFromSurface(mainGameRenderer, textSurface);
        SDL_FreeSurface(textSurface);

        if (!textTexture) {
            SDL_Log("Failed to create texture: %s", SDL_GetError());
            TTF_CloseFont(font);
            return;
        }

        // Define position for rendering
        SDL_Rect textRect = { 50, 50, 24*5, 24 };

        // Render the text
        SDL_RenderCopy(mainGameRenderer, textTexture, nullptr, &textRect);

        SDL_RenderPresent(mainGameRenderer);
    }
