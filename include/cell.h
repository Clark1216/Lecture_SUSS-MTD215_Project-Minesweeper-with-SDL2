#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include "button.h"

//A Cell is a button
class Cell : public Button {
    private:

    public:
        Cell(const SDL_Rect& rect, const SDL_Color& colour, TTF_Font* const font, const SDL_Color& fontColour);
};