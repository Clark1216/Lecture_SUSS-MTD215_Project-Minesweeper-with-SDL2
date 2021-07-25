#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>

#include "boardDetails.h"
#include "difficultyButton.h"

#define MENU_BUTTON_COUNT 3

//Menu class contains difficulty buttons
class Menu {
    private:
        DifficultyButton mDifficultyButtons[MENU_BUTTON_COUNT];
        DifficultyButton* mChosenButton;

    public:
        Menu(BoardDetails allBoardDetails[MENU_BUTTON_COUNT], SDL_Texture* textures[MENU_BUTTON_COUNT], const int BUTTON_GAP, const int BUTTON_WIDTH, const int BUTTON_HEIGHT, const SDL_Color BUTTON_COLOUR);
        void handleMouseDown(const SDL_Event& event, bool& updateFlag);
        bool handleState(BoardDetails& boardDetails);
        void render(SDL_Renderer* renderer);
        
};