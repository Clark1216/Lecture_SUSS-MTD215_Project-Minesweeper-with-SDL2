#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include "button.h"

//A Cell is a button
class Cell : public Button {
    public:
        static const int sWIDTH;
        static const int sHEIGHT;
        static const int sGAP;

        static const SDL_Color sCOLOUR;
        static const SDL_Color sPRESSED_COLOUR;

        static const int sFONT_SIZE;
        static TTF_Font* sFont;

	    static const SDL_Color sCOLOUR_OF_NUMBERS[9];

        static SDL_Texture* sTextureOfNumbers[9];
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