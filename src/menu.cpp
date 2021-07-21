#include "menu.h"

Menu::Menu(BoardDetails allBoardDetails[NUMBER_OF_BUTTONS], SDL_Texture* buttonTextures[NUMBER_OF_BUTTONS], const int BUTTON_GAP, const int BUTTON_WIDTH, const int BUTTON_HEIGHT, const SDL_Color BUTTON_COLOUR)
    : mChosenButton(nullptr) {

    //Starting coordinates
	int x = BUTTON_GAP;
	int y = BUTTON_GAP;
	//Create buttons
	for (int i = 0; i < NUMBER_OF_BUTTONS; ++i) {
        SDL_Rect rect = {x, y, BUTTON_WIDTH, BUTTON_HEIGHT};
        DifficultyButton button(rect, BUTTON_COLOUR);
        button.setTexture(buttonTextures[i]);
        button.boardDetails = allBoardDetails[i];
        mDifficultyButtons[i] = button;
        x += BUTTON_WIDTH + BUTTON_GAP;
		y = BUTTON_GAP;
	}
}

void Menu::handleMouseDown(const SDL_Event& event, bool& renderFlag) {
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    for (auto& button : mDifficultyButtons) {
        if (button.isMouseInside(mouseX, mouseY)) {
            if (event.button.button == SDL_BUTTON_LEFT) {
                mChosenButton = &button;
            }
        }
    }
}

bool Menu::handleState(BoardDetails& boardDetails) {
    if (mChosenButton != nullptr) {
        boardDetails = mChosenButton->boardDetails;
        std::cout << boardDetails.bombs << std::endl;
        return true;
    }
    return false;
}

void Menu::render(SDL_Renderer* renderer) {
    for (auto& button : mDifficultyButtons) {
        button.render(renderer);
    }
}