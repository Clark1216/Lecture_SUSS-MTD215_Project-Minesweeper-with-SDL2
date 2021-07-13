#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include "button.h"

//A Cell is a button
class Cell : public Button {
    public:
        static SDL_Color sCellColour;
        static SDL_Color sPressedCellColour;
        static SDL_Texture** sTextureOfNumbers;
        static SDL_Texture* sFlagTexture;
        static SDL_Texture* sBombTexture;

    private:
        bool mBomb;
        int mNumber;
        bool mPressed;
        bool mFlag;

    public:
        Cell(const SDL_Rect& rect, const SDL_Color& colour);
        void plantBomb();
        void plantNumber(int number);
        void setPressed();
        void setFlag();
};