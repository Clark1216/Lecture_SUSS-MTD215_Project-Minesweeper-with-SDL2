#pragma once
#include <SDL.h>
#include "button.h"
#include "boardDetails.h"

struct DifficultyButton : public Button {
    BoardDetails boardDetails;
    DifficultyButton() : Button() {}
    DifficultyButton(const SDL_Rect& rect, const SDL_Color& colour) : Button(rect, colour) {}
};