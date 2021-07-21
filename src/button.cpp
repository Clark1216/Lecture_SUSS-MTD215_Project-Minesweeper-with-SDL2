#include "button.h"

Button::Button()
: UI_element() {}

Button::Button(const SDL_Rect& rect, const SDL_Color& colour)
: UI_element(rect, colour) {}

bool Button::isMouseInside(const int x, const int y) const {
    //Assume mouse is inside tile
    bool inside = true;
	const SDL_Rect rect = getRect();

   	//Mouse is left of the button
	if (x < rect.x)
	{
		inside = false;
	}
	//Mouse is right of the button
	else if (x > rect.x + rect.w)
	{
		inside = false;
	}
	//Mouse above the button
	else if (y < rect.y)
	{
		inside = false;
	}
	//Mouse below the button
	else if (y > rect.y + rect.h)
	{
		inside = false;
	}

    return inside;
}