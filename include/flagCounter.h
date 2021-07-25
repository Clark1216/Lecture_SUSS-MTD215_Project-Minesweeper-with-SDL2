#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <string>
#include "loadTexture.h"

#define NUMBER_OF_DIGITS 10

//Keeps track of flags remaining
class FlagCounter {
    private:
        //The counter itself
        int mCounter;
        int mMaxCounter;

        //Create the main background rect to encompass the textures
        SDL_Rect mRect;
        SDL_Color mColour;
        
        //Textures for the digits (0 - 9)
        SDL_Texture* mDigitTextures[NUMBER_OF_DIGITS];

        //Rexture and rect for first digit
        SDL_Texture* mFirstDigitTexture;
        SDL_Rect mFirstDigitRect;

        //Texture and rect for seocnd digit
        SDL_Texture* mSecondDigitTexture;
        SDL_Rect mSecondDigitRect;

        //Update textures for counter
        void update();

    public:
        FlagCounter();
        FlagCounter(const int initialCount, SDL_Renderer* renderer, const SDL_Rect& rect, const SDL_Color& colour);
        void incrementCounter();
        void decrementCounter();
        int getCounter();
        void render(SDL_Renderer* renderer) const;
        void free();

};
