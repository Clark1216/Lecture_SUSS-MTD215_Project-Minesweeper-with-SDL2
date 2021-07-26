#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <time.h>
#include <string>
#include <iostream>
#include "loadTexture.h"

#define NUMBER_OF_DIGITS 10

//A 3 digit timer (max 999 seconds)
class Timer {
    private:
        //Start time and time elapsed
        int mStartTime;
        int mTimeElapsed;
        bool mRunning;

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

        //Texture and rect for third digit
        SDL_Texture* mThirdDigitTexture;
        SDL_Rect mThirdDigitRect;

        //Update textures
        void updateTextures();

    public:
        Timer();
        Timer(SDL_Renderer* renderer, const SDL_Rect& rect, const SDL_Color& colour);
        void start();
        void stop(const char* string);
        bool update();
        void render(SDL_Renderer* renderer);
        void free();

};