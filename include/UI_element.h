#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>

class UI_element {
    protected:
        SDL_Rect mRect;
        SDL_Color mColour;

        // Texture for text
        SDL_Texture* mTexture;
        SDL_Rect mTextureRect;

        void centerText();

    public:
        UI_element(const SDL_Rect& rect, const SDL_Color& colour);

        void setTexture(SDL_Texture* texture);
        void render(SDL_Renderer* renderer) const;
        void free();

};