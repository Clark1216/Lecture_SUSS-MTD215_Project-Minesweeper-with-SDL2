#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>

#include "boardDetails.h"
#include "difficultyButton.h"

#define NUMBER_OF_BUTTONS 3

//Menu class contains difficulty buttons
class Menu {
    private:
        DifficultyButton mDifficultyButtons[NUMBER_OF_BUTTONS];
        DifficultyButton* mChosenButton;

    public:
        Menu(BoardDetails allBoardDetails[NUMBER_OF_BUTTONS], SDL_Texture* buttonTextures[NUMBER_OF_BUTTONS], const int BUTTON_GAP, const int BUTTON_WIDTH, const int BUTTON_HEIGHT, const SDL_Color BUTTON_COLOUR);
        void handleMouseDown(const SDL_Event& event, bool& renderFlag);
        bool handleState(BoardDetails& boardDetails);
        void render(SDL_Renderer* renderer);
        
};