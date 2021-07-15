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
        bool mOpen;
        bool mFlag;

    public:
        Cell(const SDL_Rect& rect, const SDL_Color& colour);
        
        //Setters
        void plantBomb();
        void plantNumber(const int number);
        void open();
        void setFlag();

        //Getters
        bool isOpen();
        bool bombPlanted();
        bool numberPlanted();
};