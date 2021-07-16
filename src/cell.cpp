#include "cell.h"

const int Cell::sWIDTH = 35;
const int Cell::sHEIGHT = 35;
const int Cell::sGAP = 4;

const SDL_Color Cell::sCOLOUR = {158, 158, 158, 255}; //DARK GREY
const SDL_Color Cell::sPRESSED_COLOUR = {209, 209, 209, 255}; //LIGHT GREY

const int Cell::sFONT_SIZE = 30;
TTF_Font* Cell::sFont = nullptr;

//Define colours for all 9 numbers (0-8)
const SDL_Color Cell::sCOLOUR_OF_NUMBERS[9] = 
{{  0,   0,   0,   0},  //0 = Nothing (wont be used but kept to make index make more sense)
 { 20,  57, 168, 255},  //1 = BLUE
 { 20, 148,  18, 255},  //2 = GREEN
 {179,  30,  30, 255},  //3 = RED
 {103,  28, 173, 255},  //4 = PURPLE
 {133,  20,  20, 255},  //5 = MAROON
 {  9, 179, 164, 255},  //6 = TURQUOISE
 {  0,   0,   0, 255},  //7 = BLACK
 {255, 255, 255, 255}}; //8 = WHITE


SDL_Texture* Cell::sTextureOfNumbers[9];
SDL_Texture* Cell::sFlagTexture = nullptr;
SDL_Texture* Cell::sBombTexture = nullptr;

Cell::Cell(const SDL_Rect& rect, const SDL_Color& colour)
    : Button(rect, colour), mNumber(0), mOpen(false), mFlag(false), mBomb(false) {

}

void Cell::plantBomb() {
    mBomb = true;
}

void Cell::plantNumber(const int number) {
    mNumber = number;
}

void Cell::open() {
    if (!mOpen && !mFlag) {
        mOpen = true;
        mColour = sPRESSED_COLOUR;
        setTexture(mNumber ? sTextureOfNumbers[mNumber] : mBomb ? sBombTexture : mTexture);
    }
}

void Cell::setFlag() {
    if (!mOpen) {
        mFlag = !mFlag;
        setTexture(mFlag ? sFlagTexture : nullptr);
    }
}

bool Cell::bombPlanted() {
    return mBomb;
}

bool Cell::numberPlanted() {
    return mNumber;
}

bool Cell::isOpen() {
    return mOpen;
}