#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <math.h>
#include "UI_element.h"
#include "button.h"
#include "flagCounter.h"

//HUD contians menu button, timer, flag counter and, reset button 
class HUD {
    private:
        Button mMenuButton;
        UI_element mTimer;
        FlagCounter mFlagCounter;
        Button mResetButton;

        friend class Board;

    public:
        HUD(SDL_Renderer* renderer, SDL_Texture* menuButtonTexture, SDL_Texture* resetButtonTexture, const int SCREEN_WIDTH, const int HUD_HEIGHT, const int HUD_GAP, const int STARTING_FLAG_COUNT);
        void handleMouseDown(SDL_Event& event) const;
        void render(SDL_Renderer* renderer);
        ~HUD();
    
};