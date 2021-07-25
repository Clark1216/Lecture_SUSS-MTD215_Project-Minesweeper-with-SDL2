#include "menu.h"

Menu::Menu(SDL_Renderer* renderer, const int SCREEN_WIDTH, const int SCREEN_HEIGHT) {
    //Define button dimensions, gap, and colour
    const int GAP = 10;
    const int VERTICAL_GAP_COUNT = MENU_BUTTON_COUNT + 1;
    const int HORIZONTAL_GAP_COUNT = 2;
    const int BUTTON_WIDTH = (SCREEN_WIDTH - VERTICAL_GAP_COUNT * GAP) / MENU_BUTTON_COUNT;
    const int BUTTON_HEIGHT = (SCREEN_HEIGHT - GAP * HORIZONTAL_GAP_COUNT);
    const SDL_Color BUTTON_COLOUR = {114, 166, 176, 255}; //Grey-ish blue

    //Define max rows and max columns for each difficulty and bombs
    //BoardDetails{maxRows, maxCols, bombs}
    BoardDetails easyDetails = {9, 9, 10};
    BoardDetails mediumDetails = {16, 16, 40};
    BoardDetails hardDetails = {16, 30, 99};
    BoardDetails allBoardDetails[MENU_BUTTON_COUNT] = {easyDetails, mediumDetails, hardDetails};

    //Load texture for each button based on the difficulty they represent
    const char* strings[MENU_BUTTON_COUNT] = {"Easy", "Medium", "Hard"};

    //Starting coordinates
	int x = GAP;
	int y = GAP;
	//Create buttons
	for (int i = 0; i < MENU_BUTTON_COUNT; ++i) {
        SDL_Rect rect = {x, y, BUTTON_WIDTH, BUTTON_HEIGHT};
        mDifficultyButtons[i] = DifficultyButton(renderer, allBoardDetails[i], strings[i], rect, BUTTON_COLOUR);
        x += BUTTON_WIDTH + GAP;
		y = GAP;
	}
}

void Menu::handleMouseDown(const SDL_Event& event, const std::function<void(const BoardDetails&)>& handleDifficulty) {
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    for (auto& button : mDifficultyButtons) {
        if (button.isMouseInside(mouseX, mouseY)) {
            if (event.button.button == SDL_BUTTON_LEFT) {
                handleDifficulty(button.getBoardDetails());
                break;
            }
        }
    }
}

void Menu::render(SDL_Renderer* renderer) {
    for (auto& button : mDifficultyButtons) {
        button.render(renderer);
    }
}

Menu::~Menu() {
    for (int i = 0; i < MENU_BUTTON_COUNT; ++i) {
        mDifficultyButtons[i].free();
	}
}