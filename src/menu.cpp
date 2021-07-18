#include "menu.h"

Menu::Menu()
    : mEasyTexture(nullptr), mMediumTexture(nullptr), mHardTexture(nullptr), mChosenButton(nullptr) {

}

void Menu::createButtons(SDL_Renderer* renderer) {
    //load font
    const int FONT_SIZE = 30;
    SDL_Color FONT_COLOUR = {255, 255, 255, 255}; //White
	TTF_Font* font = loadFont(FONT_SIZE);

    //Load texture
    SDL_Texture* mEasy = loadTexture(renderer, font, FONT_COLOUR, "Easy");
    SDL_Texture* mMedium = loadTexture(renderer, font, FONT_COLOUR, "Medium");
    SDL_Texture* mHard = loadTexture(renderer, font, FONT_COLOUR, "Hard");
    SDL_Texture* menuTextures[3] = {mEasy, mMedium, mHard};

    //Define button attributes
    const int MAX_BUTTONS = 3;
    const int BUTTON_GAP = 10;
    const int BUTTON_WIDTH = 153;
    const int BUTTON_HEIGHT = 130;
    const SDL_Color BUTTON_COLOUR = {145, 145, 145, 255};

    //Define max rows and max columns for each difficulty and bombs
    BoardDetails easy(9, 9, 10);
    BoardDetails medium(16, 16, 40);
    BoardDetails hard(16, 30, 99);
    BoardDetails boardDetails[MAX_BUTTONS] = {easy, medium, hard};

    //Starting coordinates
	int x = BUTTON_GAP;
	int y = BUTTON_GAP;
	//Create buttons
	for (int i = 0; i < MAX_BUTTONS; ++i) {
        SDL_Rect rect = {x, y, BUTTON_WIDTH, BUTTON_HEIGHT};
        DifficultyButton button(rect, BUTTON_COLOUR);
        button.setTexture(menuTextures[i]);
        button.boardDetails = boardDetails[i];
        mDifficultyButtons.push_back(button);
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
        return true;
    }
    return false;
}

void Menu::render(SDL_Renderer* renderer) {
    for (auto& button : mDifficultyButtons) {
        button.render(renderer);
    }
}

Menu::~Menu() {
    SDL_DestroyTexture(mEasyTexture);
	SDL_DestroyTexture(mMediumTexture);
    SDL_DestroyTexture(mHardTexture);
	mEasyTexture = nullptr;
    mMediumTexture = nullptr;
    mHardTexture = nullptr;
}