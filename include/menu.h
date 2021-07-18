#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <vector>

#include "loadTexture.h"
#include "button.h"

//Store board size
struct BoardDetails {
    int mMaxRows;
    int mMaxCols;
    int mBombs;

    BoardDetails() : mMaxRows(0), mMaxCols(0), mBombs(0) {}
    BoardDetails(const int maxRows, const int maxCols, const int bombs)
     : mMaxRows(maxRows), mMaxCols(maxCols), mBombs(bombs) {}
};

//Wrapper around button class to hold difficulty information
struct DifficultyButton : public Button {
    DifficultyButton(const SDL_Rect& rect, const SDL_Color& colour) 
        : Button(rect, colour), boardDetails(0, 0, 0) {}

    //Store how many cells on the board
    BoardDetails boardDetails;
};

//Menu class contains difficulty buttons
class Menu {
    private:
        std::vector<DifficultyButton> mDifficultyButtons;
        SDL_Texture* mEasyTexture;
        SDL_Texture* mMediumTexture;
        SDL_Texture* mHardTexture;
        DifficultyButton* mChosenButton;

    public:
        Menu();
        void createButtons(SDL_Renderer* renderer);
        void handleMouseDown(const SDL_Event& event, bool& renderFlag);
        bool handleState(BoardDetails& boardDetails);
        void render(SDL_Renderer* renderer);
        ~Menu();
};