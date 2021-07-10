#include "button.h"

Button::Button(const SDL_Rect& rect, const SDL_Color& colour)
    : UI_element(rect, colour) {

}

bool Button::isMouseInside(const int x, const int y) const {
    //Assume mouse is inside tile
    bool inside = true;

   	//Mouse is left of the button
	if (x < mRect.x)
	{
		inside = false;
	}
	//Mouse is right of the button
	else if (x > mRect.x + mRect.w)
	{
		inside = false;
	}
	//Mouse above the button
	else if (y < mRect.y)
	{
		inside = false;
	}
	//Mouse below the button
	else if (y > mRect.y + mRect.h)
	{
		inside = false;
	}

    return inside;
}