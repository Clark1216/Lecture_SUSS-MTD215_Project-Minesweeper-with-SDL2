#include "HUD.h"

HUD::HUD(SDL_Renderer* renderer, const int SCREEN_WIDTH, const int HUD_HEIGHT, const int HUD_GAP, const int STARTING_FLAG_COUNT) {
    //Define main HUD element properties
    const int UI_ELEMENT_COUNT = 4;
    const int TOTAL_WIDTH_REMAINING = SCREEN_WIDTH - UI_ELEMENT_COUNT * (HUD_GAP + 1);
    const SDL_Color UI_ELEMENT_COLOUR = {114, 166, 176, 255}; // Grey-ish blue;
    
    //Load Texture for menu and reset button
    //Load font
    const int FONT_SIZE = 25;
    SDL_Color FONT_COLOUR = {255, 255, 255, 255}; //White
    TTF_Font* font = loadFont(FONT_SIZE);

    //Load texture for each button based on the difficulty they represent
    SDL_Texture* menuButtonTexture = loadTexture(renderer, font, FONT_COLOUR, "Menu");
    SDL_Texture* resetButtonTexture = loadTexture(renderer, font, FONT_COLOUR, "Reset");

    //Close font
    TTF_CloseFont(font);
    font = nullptr;

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
    mFlagCounter = FlagCounter(STARTING_FLAG_COUNT, renderer, rect, UI_ELEMENT_COLOUR);
    x += width + HUD_GAP;

    fraction = 0.25;
    width = round(TOTAL_WIDTH_REMAINING * fraction);
    rect = {x, y, width, HUD_HEIGHT};
    mResetButton = Button(rect, UI_ELEMENT_COLOUR);
    mResetButton.setTexture(resetButtonTexture);

}

void HUD::handleMouseDown(SDL_Event& event, const std::function<void()>& handleMenuEvent, const std::function<void()>& handleResetEvent) const {
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    if (event.button.button == SDL_BUTTON_LEFT) {
        if (mMenuButton.isMouseInside(mouseX, mouseY)) {
            handleMenuEvent();
        } else if (mResetButton.isMouseInside(mouseX, mouseY)) {
            handleResetEvent();
        }
    }
}

void HUD::render(SDL_Renderer* renderer) {
    mMenuButton.render(renderer);
    mTimer.render(renderer);
    mFlagCounter.render(renderer);
    mResetButton.render(renderer);
}

HUD::~HUD() {
    mMenuButton.free();
    mResetButton.free();
    mFlagCounter.free();
}