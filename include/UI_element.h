#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>

class UI_element {
    protected:
        SDL_Rect mRect;
        SDL_Color mColour;

        // Texture for text
        TTF_Font* mFont;
        SDL_Rect mFontRect;
        SDL_Color mFontColour;
        SDL_Texture* mTexture;

        void centerText();

    public:
        UI_element(const SDL_Rect& rect, const SDL_Color& colour, TTF_Font* font, const SDL_Color& fontColour);

        void loadTexture(SDL_Renderer* renderer, const char* text);
        void render(SDL_Renderer* renderer) const;
        void free();

};