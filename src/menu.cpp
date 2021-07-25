#include "menu.h"

Menu::Menu(BoardDetails allBoardDetails[MENU_BUTTON_COUNT], SDL_Texture* textures[MENU_BUTTON_COUNT], const int BUTTON_GAP, const int BUTTON_WIDTH, const int BUTTON_HEIGHT, const SDL_Color BUTTON_COLOUR)
    : mChosenButton(nullptr) {
    
    //Starting coordinates
	int x = BUTTON_GAP;
	int y = BUTTON_GAP;
	//Create buttons
	for (int i = 0; i < MENU_BUTTON_COUNT; ++i) {
        SDL_Rect rect = {x, y, BUTTON_WIDTH, BUTTON_HEIGHT};
        DifficultyButton button(rect, BUTTON_COLOUR);
        button.setTexture(textures[i]);
        button.boardDetails = allBoardDetails[i];
        mDifficultyButtons[i] = button;
        x += BUTTON_WIDTH + BUTTON_GAP;
		y = BUTTON_GAP;
	}
}

void Menu::handleMouseDown(const SDL_Event& event, bool& updateFlag) {
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    for (auto& button : mDifficultyButtons) {
        if (button.isMouseInside(mouseX, mouseY)) {
            if (event.button.button == SDL_BUTTON_LEFT) {
                mChosenButton = &button;
                updateFlag = true;
            }
        }
    }
}

bool Menu::handleState(BoardDetails& boardDetails) {
    if (mChosenButton != nullptr) {
        boardDetails = mChosenButton->boardDetails;
        return true;
    }
    return false;
}

void Menu::render(SDL_Renderer* renderer) {
    for (auto& button : mDifficultyButtons) {
        button.render(renderer);
    }
}