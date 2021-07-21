#pragma once
#include <SDL.h>
#include "UI_element.h"

//A Button is a UI_element
class Button : public UI_element {
    public:
        Button();
        Button(const SDL_Rect& rect, const SDL_Color& colour);
        bool isMouseInside(const int x, const int y) const;
};