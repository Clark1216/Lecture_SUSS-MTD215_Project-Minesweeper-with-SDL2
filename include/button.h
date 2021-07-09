#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include "UI_element.h"

class Button : public UI_element {
    public:
        Button(const SDL_Rect& rect, const SDL_Color& colour, TTF_Font* const font, const SDL_Color& fontColour);
        bool isMouseInside(const int x, const int y) const;
};