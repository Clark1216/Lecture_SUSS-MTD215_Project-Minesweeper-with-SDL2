#pragma once
#include <iostream>
#include <SDL.h>
#include "button.h"
#define NUMBER_COUNT 9

//A Cell is a button
class Cell : public Button {
    public:
        static SDL_Color sCOLOUR;
        static SDL_Color sPRESSED_COLOUR;

        static SDL_Texture* sTextureOfNumbers[NUMBER_COUNT];
        static SDL_Texture* sFlagTexture;
        static SDL_Texture* sBombTexture;

    private:
        bool mBomb;
        int mNumber;
        bool mOpen;
        bool mFlag;

        void checkTexturesExist();
        friend class Board;

    public:
        Cell();
        Cell(const SDL_Rect& rect);
        
        //Setters
        void plantBomb();
        void plantNumber(const int number);
        void open();
        bool setFlag();

        //Getters
        bool isOpen();
        bool bombPlanted();
        bool numberPlanted();
};