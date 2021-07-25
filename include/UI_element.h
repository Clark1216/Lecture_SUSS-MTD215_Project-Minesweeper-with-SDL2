#pragma once
#include <SDL.h>

//Renders a coloured rect with a texture
class UI_element {
    private:
        SDL_Rect mRect;
        SDL_Color mColour;

        SDL_Texture* mTexture;
        SDL_Rect mTextureRect;

        void centerTexture();
        
    public:
        UI_element();
        UI_element(const SDL_Rect& rect, const SDL_Color& colour);
        void setTexture(SDL_Texture* texture);
        void changeColourTo(const SDL_Color& colour);
        SDL_Rect getRect() const ;
        void render(SDL_Renderer* renderer) const;
        void free();
};