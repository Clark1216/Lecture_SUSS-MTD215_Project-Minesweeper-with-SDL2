#include "timer.h"

Timer::Timer()
    : mStartTime((int)time(NULL)), mTimeElapsed(0), mRunning(false),
    mRect({0, 0, 0, 0}), mColour({0, 0, 0, 0}),
    mDigitTextures(),
    mFirstDigitTexture(nullptr), mFirstDigitRect({0, 0, 0, 0}),
    mSecondDigitTexture(nullptr), mSecondDigitRect({0, 0, 0, 0}),
    mThirdDigitTexture(nullptr), mThirdDigitRect({0, 0, 0, 0}) {}

Timer::Timer(SDL_Renderer* renderer, const SDL_Rect& rect, const SDL_Color& colour)
    : mStartTime((int)time(NULL)), mTimeElapsed(0), mRunning(false),
    mRect(rect), mColour(colour),
    mDigitTextures(),
    mFirstDigitTexture(nullptr), mFirstDigitRect({0, 0, 0, 0}),
    mSecondDigitTexture(nullptr), mSecondDigitRect({0, 0, 0, 0}),
    mThirdDigitTexture(nullptr), mThirdDigitRect({0, 0, 0, 0}) {

    //load font
    const int FONT_SIZE = 30;					
    TTF_Font* font = loadFont(FONT_SIZE);
    const SDL_Color FONT_COLOUR = {255, 255, 255, 255}; //White

    //Load textures for numbers for flag timer
    for (int i = 0; i < NUMBER_OF_DIGITS; ++i) {
        mDigitTextures[i] = loadTexture(renderer, font, FONT_COLOUR, std::to_string(i).c_str());
    }

    //Close font
    TTF_CloseFont(font);
    font = nullptr;

}

void Timer::updateTextures() {
    if (mTimeElapsed > 999) {
        std::cout << "Warning: Time elapsed greater than 999 seconds!" << std::endl;
        //Cap time elapsed at 999
        mTimeElapsed = 999;
    }

    //Convert timer into 3 digits
    const int digit1 = mTimeElapsed / 100;
    const int digit2 = mTimeElapsed / 10;
    const int digit3 = mTimeElapsed % 10;

    //Query the texture for rect
    mFirstDigitTexture = mDigitTextures[digit1];
    SDL_QueryTexture(mFirstDigitTexture, NULL, NULL, &mFirstDigitRect.w, &mFirstDigitRect.h);
    mSecondDigitTexture = mDigitTextures[digit2];
    SDL_QueryTexture(mSecondDigitTexture, NULL, NULL, &mSecondDigitRect.w, &mSecondDigitRect.h);
    mThirdDigitTexture = mDigitTextures[digit3];
    SDL_QueryTexture(mThirdDigitTexture, NULL, NULL, &mThirdDigitRect.w, &mThirdDigitRect.h);

    //Center by y plane
    mFirstDigitRect.y = mRect.y + 0.5 * (mRect.h - mFirstDigitRect.h);
    mSecondDigitRect.y = mRect.y + 0.5 * (mRect.h - mSecondDigitRect.h);
    mThirdDigitRect.y = mRect.y + 0.5 * (mRect.h - mThirdDigitRect.h);

    //Center by x plane
    const int totalWidthOfDigitRects = mFirstDigitRect.w + mSecondDigitRect.w + mThirdDigitRect.w;
    mFirstDigitRect.x = mRect.x + 0.5 * (mRect.w - totalWidthOfDigitRects);
    mSecondDigitRect.x = mFirstDigitRect.x + mFirstDigitRect.w;
    mThirdDigitRect.x = mSecondDigitRect.x + mSecondDigitRect.w;

}

void Timer::start() {
    mStartTime = (int)time(NULL);
    mTimeElapsed = 0;
    mRunning = true;

    //Update textures
    updateTextures();    
}

void Timer::stop(const char* string) {
    mRunning = false;
    //Change colour to red
    if (strcmp(string, "RED") == 0)
        mColour = {252, 53, 3, 255};
    else if (strcmp(string, "GREEN") == 0) 
        mColour = {33, 189, 6, 255};
}

bool Timer::update() {
    if (mRunning) {
        //Get time difference
        const int mDifference = (int)time(NULL) - mStartTime;

        //Only update time elapsed if it has changed
        if (mDifference > mTimeElapsed) {
            mTimeElapsed = mDifference;
            updateTextures();
            return true;
        }
    }

    return false;
};

void Timer::render(SDL_Renderer* renderer) {
    //Draw background rect
    SDL_SetRenderDrawColor(renderer, mColour.r, mColour.g, mColour.b, mColour.a);
    SDL_RenderFillRect(renderer, &mRect);

    //Render first digit if it exists
    if (mFirstDigitTexture != nullptr) {
        SDL_RenderCopy(renderer, mFirstDigitTexture, NULL, &mFirstDigitRect);
    }

    //Render second digit if it exists
    if (mSecondDigitTexture != nullptr) {
        SDL_RenderCopy(renderer, mSecondDigitTexture, NULL, &mSecondDigitRect);
    }

    //Render second digit if it exists
    if (mThirdDigitTexture != nullptr) {
        SDL_RenderCopy(renderer, mThirdDigitTexture, NULL, &mThirdDigitRect);
    }
}

void Timer::free() {
    for (int i = 0; i < NUMBER_OF_DIGITS; ++i) {
        if (mDigitTextures[i] != nullptr) {
            SDL_DestroyTexture(mDigitTextures[i]);
            mDigitTextures[i] = nullptr;
        }
    }
}