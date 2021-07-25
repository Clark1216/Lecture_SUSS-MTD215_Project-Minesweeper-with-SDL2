#include "HUD.h"

HUD::HUD(SDL_Renderer* renderer, SDL_Texture* menuButtonTexture, SDL_Texture* resetButtonTexture, const int SCREEN_WIDTH, const int HUD_HEIGHT, const int HUD_GAP, const int STARTING_FLAG_COUNT) {
    //Define main HUD element properties
    const int UI_ELEMENT_COUNT = 4;
    const int TOTAL_WIDTH_REMAINING = SCREEN_WIDTH - UI_ELEMENT_COUNT * (HUD_GAP + 1);
    const SDL_Color UI_ELEMENT_COLOUR = {114, 166, 176, 255}; // Grey-ish blue;
    
    //Setup each ui element
    int x = HUD_GAP;
    int y = HUD_GAP;

    //Create menu button
    float fraction = 0.25;
    int width = round(TOTAL_WIDTH_REMAINING * fraction);
    SDL_Rect rect = {x, y, width, HUD_HEIGHT};
    mMenuButton = Button(rect, UI_ELEMENT_COLOUR);
    mMenuButton.setTexture(menuButtonTexture);
    x += width + HUD_GAP;

    fraction = 0.25;
    width = round(TOTAL_WIDTH_REMAINING * fraction);
    rect = {x, y, width, HUD_HEIGHT};
    mTimer = UI_element(rect, UI_ELEMENT_COLOUR);
    x += width + HUD_GAP;

    fraction = 0.25;
    width = round(TOTAL_WIDTH_REMAINING * fraction);
    rect = {x, y, width, HUD_HEIGHT};
    int fontSize = 25;
    mFlagCounter = FlagCounter(STARTING_FLAG_COUNT, renderer, fontSize, rect, UI_ELEMENT_COLOUR);
    x += width + HUD_GAP;

    fraction = 0.25;
    width = round(TOTAL_WIDTH_REMAINING * fraction);
    rect = {x, y, width, HUD_HEIGHT};
    mResetButton = Button(rect, UI_ELEMENT_COLOUR);
    mResetButton.setTexture(resetButtonTexture);

}

void HUD::handleMouseDown(SDL_Event& event) const{
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    if (mMenuButton.isMouseInside(mouseX, mouseY)) {
        std::cout << "Menu button pressed" << std::endl;
    } else if (mResetButton.isMouseInside(mouseX, mouseY)) {
        std::cout << "Reset button pressed" << std::endl;
    }
}

void HUD::render(SDL_Renderer* renderer) {
    mMenuButton.render(renderer);
    mTimer.render(renderer);
    mFlagCounter.render(renderer);
    mResetButton.render(renderer);
}

HUD::~HUD() {
    //Free textures in flag counter
    mFlagCounter.free();
}