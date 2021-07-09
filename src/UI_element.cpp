#include "UI_element.h"

UI_element::UI_element(const SDL_Rect& rect, const SDL_Color& colour, TTF_Font* font, const SDL_Color& fontColour) 
    : mRect(rect), mColour(colour), 
      mFontRect({0, 0, 0, 0}), mFontColour(fontColour), mFont(font), 
      mTexture(nullptr) {
    
}

void UI_element::loadTexture(SDL_Renderer* renderer, const char* text) {
    // Free old texture if it exists
    free(); 

    SDL_Surface* textSurface = TTF_RenderText_Solid(mFont, text, mFontColour);
    if (textSurface == nullptr) {
        std::cout << "Unable to render text surface! Error: " << TTF_GetError() << std::endl; 
    } else {
        mTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        if (mTexture == nullptr) {
            std::cout << "Unable to create texture form rendered text! Error: " << SDL_GetError() << std::endl;
        } else {
            mFontRect.w = textSurface->w;
            mFontRect.h = textSurface->h;
        }
        SDL_FreeSurface(textSurface);
    }
    centerText();
}

void UI_element::centerText() {
    // Center text on UI_element
    mFontRect.y = mRect.y + 0.5 * (mRect.h - mFontRect.h);
	mFontRect.x = mRect.x + 0.5 * (mRect.w - mFontRect.w);
}                                                                                                                                                                                                                                                                            

void UI_element::render(SDL_Renderer* renderer) const {
    // Render UI_element
    SDL_SetRenderDrawColor(renderer, mColour.r, mColour.g, mColour.b, mColour.a);
    SDL_RenderFillRect(renderer, &mRect);

    // Render texture if it exists
    if (mTexture != nullptr) {
        SDL_RenderCopy(renderer, mTexture, nullptr, &mFontRect);
    }
}

void UI_element::free() {
    if (mTexture != nullptr) {
        SDL_DestroyTexture(mTexture);
        mTexture = nullptr;
    }
}