#include "flagCounter.h"

FlagCounter::FlagCounter() 
    : mCounter(0), mMaxCounter(99),
    mRect({0, 0, 0, 0}), mColour({0, 0, 0 ,0}),
    mDigitTextures(),
    mFirstDigitTexture(nullptr), mFirstDigitRect({0, 0, 0, 0}),
    mSecondDigitTexture(nullptr), mSecondDigitRect({0, 0, 0, 0})  {}

FlagCounter::FlagCounter(const int initialCount, SDL_Renderer* renderer, const SDL_Rect& rect, const SDL_Color& colour) 
    : mCounter(initialCount), mMaxCounter(99),
    mRect(rect), mColour(colour),
    mDigitTextures(),
    mFirstDigitTexture(nullptr), mFirstDigitRect({0, 0, 0, 0}),
    mSecondDigitTexture(nullptr), mSecondDigitRect({0, 0, 0, 0}) {

    //Check counter is lower than maximum count
    if (mCounter > mMaxCounter) {
        std::cout << "Warning: Number greater than 99! Make sure it is equal or less than this" << std::endl; 
    }

    //load font
    const int FONT_SIZE = 30;					
    TTF_Font* font = loadFont(FONT_SIZE);
    const SDL_Color FONT_COLOUR = {191, 8, 17, 255}; //Red

    //Load textures for numbers for flag counter and timer
    for (int i = 0; i < NUMBER_OF_DIGITS; ++i) {
        mDigitTextures[i] = loadTexture(renderer, font, FONT_COLOUR, std::to_string(i).c_str());
    }

    //Close font
    TTF_CloseFont(font);
    font = nullptr;

    //Update counter textures
    update();    
}

void FlagCounter::update() {
    //Convert counter into 2 digits
    const int digit1 = mCounter / 10;
    const int digit2 = mCounter % 10;

    //Query the texture for rect
    mFirstDigitTexture = mDigitTextures[digit1];
    SDL_QueryTexture(mFirstDigitTexture, NULL, NULL, &mFirstDigitRect.w, &mFirstDigitRect.h);

    mSecondDigitTexture = mDigitTextures[digit2];
    SDL_QueryTexture(mSecondDigitTexture, NULL, NULL, &mSecondDigitRect.w, &mSecondDigitRect.h);

    //Center by y plane
    mFirstDigitRect.y = mRect.y + 0.5 * (mRect.h - mFirstDigitRect.h);
    mSecondDigitRect.y = mRect.y + 0.5 * (mRect.h - mSecondDigitRect.h);

    //Center by x plane
    const int totalWidthOfDigitRects = mFirstDigitRect.w + mSecondDigitRect.w;
	mFirstDigitRect.x = mRect.x + 0.5 * (mRect.w - totalWidthOfDigitRects);
    mSecondDigitRect.x = mRect.x + 0.5 * mRect.w;
};

void FlagCounter::incrementCounter() {
    mCounter++;
    update();
}

void FlagCounter::decrementCounter() {
    mCounter--;
    update();
}

int FlagCounter::getCounter() {
    return mCounter;
}

void FlagCounter::render(SDL_Renderer* renderer) const {
    //Render UI_element
    SDL_SetRenderDrawColor(renderer, mColour.r, mColour.g, mColour.b, mColour.a);
    SDL_RenderFillRect(renderer, &mRect);

    //Render first digit texture if it exists
    if (mFirstDigitTexture != nullptr) {
        SDL_RenderCopy(renderer, mFirstDigitTexture, NULL, &mFirstDigitRect);
    }

    //Render second digit texture if it exists
    if (mSecondDigitTexture != nullptr) {
        SDL_RenderCopy(renderer, mSecondDigitTexture, NULL, &mSecondDigitRect);
    }
}

void FlagCounter::free() {
    //Destroy textures
    for (int i = 0; i < NUMBER_OF_DIGITS; ++i) {
        if (mDigitTextures[i] != nullptr) {
            SDL_DestroyTexture(mDigitTextures[i]);
            mDigitTextures[i] = nullptr;
        }
    }
}