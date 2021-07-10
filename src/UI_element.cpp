#include "UI_element.h"

UI_element::UI_element(const SDL_Rect& rect, const SDL_Color& colour) 
    : mRect(rect), mColour(colour), mTexture(nullptr), mTextureRect({0, 0, 0, 0}) {
    
}

void UI_element::setTexture(SDL_Texture* texture) {
    mTexture = texture;
    SDL_QueryTexture(texture, NULL, NULL, &mTextureRect.w, &mTextureRect.h);
    centerText();
}

void UI_element::centerText() {
    // Center text on UI_element
    mTextureRect.y = mRect.y + 0.5 * (mRect.h - mTextureRect.h);
	mTextureRect.x = mRect.x + 0.5 * (mRect.w - mTextureRect.w);
}                                                                                                                                                                                                                                                                            

void UI_element::render(SDL_Renderer* renderer) const {
    // Render UI_element
    SDL_SetRenderDrawColor(renderer, mColour.r, mColour.g, mColour.b, mColour.a);
    SDL_RenderFillRect(renderer, &mRect);

    // Render texture if it exists
    if (mTexture != nullptr) {
        SDL_RenderCopy(renderer, mTexture, nullptr, &mTextureRect);
    }
}