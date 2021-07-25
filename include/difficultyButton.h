#pragma once
#include <SDL.h>
#include "loadTexture.h"
#include "button.h"
#include "boardDetails.h"

class DifficultyButton : public Button {
    private:
        BoardDetails mBoardDetails;

    public:
        DifficultyButton();
        DifficultyButton(SDL_Renderer* renderer, const BoardDetails& boardDetails, const char* string, const SDL_Rect& rect, const SDL_Color& colour);
        BoardDetails getBoardDetails();
};