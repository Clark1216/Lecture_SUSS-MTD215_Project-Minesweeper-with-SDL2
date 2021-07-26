#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <functional>
#include <iostream>
#include <math.h>

#include "button.h"
#include "timer.h"
#include "flagCounter.h"

//HUD contians menu button, timer, flag counter and, reset button 
class HUD {
    private:
        Button mMenuButton;
        Timer mTimer;
        FlagCounter mFlagCounter;
        Button mResetButton;

        friend class Board;

    public:
        HUD(SDL_Renderer* renderer, const int SCREEN_WIDTH, const int HUD_HEIGHT, const int HUD_GAP, const int STARTING_FLAG_COUNT);
        void handleMouseDown(SDL_Event& event, const std::function<void()>& handleMenuEvent, const std::function<void()>& handleResetEvent) const;
        void startTimer();
        bool updateTimer();
        void render(SDL_Renderer* renderer);
        ~HUD();
    
};