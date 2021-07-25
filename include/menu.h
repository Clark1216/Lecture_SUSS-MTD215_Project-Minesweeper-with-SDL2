#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <functional>
#include <iostream>

#include "loadTexture.h"
#include "boardDetails.h"
#include "difficultyButton.h"

#define MENU_BUTTON_COUNT 3

//Menu class contains difficulty buttons
class Menu {
    private:
        DifficultyButton mDifficultyButtons[MENU_BUTTON_COUNT];

    public:
        Menu(SDL_Renderer* renderer, const int SCREEN_WIDTH, const int SCREEN_HEIGHT);
        void handleMouseDown(const SDL_Event& event, const std::function<void(const BoardDetails&)>& handleDifficulty);
        void render(SDL_Renderer* renderer);
        ~Menu();
        
};