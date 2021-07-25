#include "difficultyButton.h"

DifficultyButton::DifficultyButton() : mBoardDetails({0, 0, 0}) {}

DifficultyButton::DifficultyButton(SDL_Renderer* renderer, const BoardDetails& boardDetails, const char* string, const SDL_Rect& rect, const SDL_Color& colour) 
: Button(rect, colour), mBoardDetails(boardDetails) {
    //load font
    const int FONT_SIZE = 30;
    SDL_Color FONT_COLOUR = {255, 255, 255, 255}; //White
	TTF_Font* font = loadFont(FONT_SIZE);

    //Load texture for each button based on the difficulty they represent
    setTexture(loadTexture(renderer, font, FONT_COLOUR, string));

    //Close font
    TTF_CloseFont(font);
    font = nullptr;    

}

BoardDetails DifficultyButton::getBoardDetails() {
    return mBoardDetails;
}