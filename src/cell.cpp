#include "cell.h"

SDL_Color Cell::sCOLOUR = {0, 0, 0, 0};
SDL_Color Cell::sPRESSED_COLOUR = {0, 0, 0, 0};

SDL_Texture* Cell::sTextureOfNumbers[NUMBER_COUNT] = {};
SDL_Texture* Cell::sFlagTexture = nullptr;
SDL_Texture* Cell::sBombTexture = nullptr;

Cell::Cell() 
: Button(), mNumber(0), mOpen(false), mFlag(false), mBomb(false) {
    checkTexturesExist();
}

Cell::Cell(const SDL_Rect& rect) 
: Button(rect, sCOLOUR), mNumber(0), mOpen(false), mFlag(false), mBomb(false) {
    checkTexturesExist();
}

void Cell::checkTexturesExist() {
    if (!sTextureOfNumbers || !sFlagTexture || !sBombTexture) {
        std::cout << "Warning no textures loaded" << std::endl;
    }
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
        changeColourTo(sPRESSED_COLOUR);
        setTexture(mNumber ? sTextureOfNumbers[mNumber] : mBomb ? sBombTexture : nullptr);
    }
}

//Flip flag bool and return result
bool Cell::setFlag() {
    if (!mOpen) {
        mFlag = !mFlag;
        setTexture(mFlag ? sFlagTexture : nullptr);
    }
    return mFlag;
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

bool Cell::getFlag() {
    return mFlag;
}