#include "minesweeper.h"

Minesweeper::Minesweeper() 
: mWindow(nullptr), mRenderer(nullptr), mGameState(MENU), mBoardDetails({0, 0, 0}) {
    //Screen dimension
    const int SCREEN_WIDTH = 500;
    const int SCREEN_HEIGHT = 150;

    //Initialise SDL
    if(SDL_Init(SDL_INIT_VIDEO) < 0 ) {
        std::cout << "SDL could not initialise! SDL_Error: " << SDL_GetError() << std::endl;
    }

    //Initialise SDL_ttf
    if (TTF_Init() < 0) {
        std::cout << "SDL_ttf could not initialise! Error: " << TTF_GetError() << std::endl;
    }

    //Create window
    mWindow = SDL_CreateWindow("Minesweeper", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (mWindow == nullptr) {
        std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
    }

    //Create renderer
    mRenderer = SDL_CreateRenderer(mWindow, -1, 0);
    if (mRenderer == nullptr) {
        std::cout << "SDL could not create renderer! Error: " << SDL_GetError() << std::endl;
    }
}

//Menu loop for choosing difficulty
void Minesweeper::menuLoop() {
    //Change size of window
    const int SCREEN_WIDTH = 500;
    const int SCREEN_HEIGHT = 150;
    SDL_SetWindowSize(mWindow, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_SetWindowPosition(mWindow, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

    //Define button desgin
    const int BUTTON_GAP = 10;
    const int BUTTON_WIDTH = 153;
    const int BUTTON_HEIGHT = 130;
    const SDL_Color BUTTON_COLOUR = {145, 145, 145, 255};

    //load font
    const int FONT_SIZE = 30;
    SDL_Color FONT_COLOUR = {255, 255, 255, 255}; //White
	TTF_Font* font = loadFont(FONT_SIZE);

    //Load texture
    const char* diffculties[NUMBER_OF_BUTTONS] = {"Easy", "Medium", "Hard"};
    SDL_Texture* buttonTextures[NUMBER_OF_BUTTONS];
    for (int i = 0; i < NUMBER_OF_BUTTONS; ++i) {
        buttonTextures[i] = loadTexture(mRenderer, font, FONT_COLOUR, diffculties[i]);
    }

    //Define max rows and max columns for each difficulty and bombs
    //BoardDetails{maxRows, maxCols, bombs}
    const int MAX_BUTTONS = 3;
    BoardDetails easyDetails = {9, 9, 10};
    BoardDetails mediumDetails = {16, 16, 40};
    BoardDetails hardDetails = {16, 30, 99};
    BoardDetails allBoardDetails[MAX_BUTTONS] = {easyDetails, mediumDetails, hardDetails};

    //Create menu
    Menu menu(allBoardDetails, buttonTextures, BUTTON_GAP, BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_COLOUR);

    //Set loop variables
    SDL_Event event;
    bool renderFlag = true;

    //Menu loop
    while (mGameState == MENU) {
        //Handle events
        while (SDL_PollEvent(&event) != 0) {
            //Handle quitting
            if (event.type == SDL_QUIT) {
                mGameState = EXIT;
            }
            //Handle mouse click
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                menu.handleMouseDown(event, renderFlag);
            }
        }

        //Handle state and return true if button was clicked
        if (menu.handleState(mBoardDetails)) {
            mGameState = BOARD;
        }

        if (renderFlag) {
            //Clear screen
            SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
            SDL_RenderClear(mRenderer);

            //Render board
            menu.render(mRenderer);

            //Update screen from backbuffer and clear backbuffer
            SDL_RenderPresent(mRenderer);

            //Reset render flag
            renderFlag = false;
        }

        //Slow down programme
        SDL_Delay(20);
    }

    //Destroy fonts and textures
    TTF_CloseFont(font);
    font = nullptr;

    for (int i = 0; i < NUMBER_OF_BUTTONS; ++i) {
        SDL_DestroyTexture(buttonTextures[i]);
        buttonTextures[i] = nullptr;
    }

}

//Board loop contianing the chosen difficulty of the game
void Minesweeper::boardLoop() {
    //Define size of cells
    const int CELL_WIDTH = 35;
    const int CELL_HEIGHT = 35;
    const int CELL_GAP = 4;

    //Change size of window based on size of cells
    const int SCREEN_WIDTH = CELL_WIDTH * mBoardDetails.cols + CELL_GAP * (mBoardDetails.cols + 1);
    const int SCREEN_HEIGHT = CELL_HEIGHT * mBoardDetails.rows + CELL_GAP* (mBoardDetails.rows + 1);
    SDL_SetWindowSize(mWindow, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_SetWindowPosition(mWindow, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

    //load font
    const int FONT_SIZE = 30;
    TTF_Font* font = loadFont(FONT_SIZE);

    //Define colours for all 9 numbers (0-8)
    const int MAX_NUMBERS = 9;
    const SDL_Color COLOUR_OF_NUMBERS[MAX_NUMBERS] = 
    {{  0,   0,   0,  0},  //0 = Nothing (wont be used but kept to make index make more sense)
    { 20,  57, 168, 255},  //1 = BLUE
    { 20, 148,  18, 255},  //2 = GREEN
    {179,  30,  30, 255},  //3 = RED
    {103,  28, 173, 255},  //4 = PURPLE
    {133,  20,  20, 255},  //5 = MAROON
    {  9, 179, 164, 255},  //6 = TURQUOISE
    {  0,   0,   0, 255},  //7 = BLACK
    {255, 255, 255, 255}}; //8 = WHITE

    //Load textures for all 9 numbers (0-8)
    //Skip 0 since it does not have a texture (default nullptr)
    for (int i = 1; i < MAX_NUMBERS; ++i) {
        Cell::sTextureOfNumbers[i] = loadTexture(mRenderer, font, COLOUR_OF_NUMBERS[i], std::to_string(i).c_str());
    }

    //Set Cell flag and bomb textures
    Cell::sFlagTexture = loadTexture(mRenderer, "assets/flag.bmp");
    Cell::sBombTexture = loadTexture(mRenderer, "assets/bomb.bmp");

    //Set Cell colours
    Cell::sCOLOUR = {158, 158, 158, 255}; //DARK GREY
    Cell::sPRESSED_COLOUR = {209, 209, 209, 255}; //LIGHT GREY
    
    //Create board
    Board board(mBoardDetails, CELL_WIDTH, CELL_HEIGHT, CELL_GAP);
    
    //Set loop variables
    SDL_Event event;
    bool renderFlag= true;

    //Board loop
    while (mGameState == BOARD) {
        //Handle events
        while (SDL_PollEvent(&event) != 0) {
            //Handle quitting
            if (event.type == SDL_QUIT) {
                mGameState = EXIT;
            }
            //Handle mouse click
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                board.handleMouseDown(event, renderFlag);
            }
        }
        
        //Handle current state of board
        board.handleState();

        if (renderFlag) {
            //Clear screen
            SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
            SDL_RenderClear(mRenderer);

            //Render board
            board.render(mRenderer);

            //Update screen from backbuffer and clear backbuffer
            SDL_RenderPresent(mRenderer);

            //Reset render flag
            renderFlag = false;
        }

        //Slow down programme
        SDL_Delay(20);
    }

    //Destroy fonts and textures
    TTF_CloseFont(font);
    SDL_DestroyTexture(Cell::sFlagTexture);
    SDL_DestroyTexture(Cell::sBombTexture);
    font = nullptr;
    Cell::sFlagTexture = nullptr;
    Cell::sBombTexture = nullptr;

    for (int i = 0; i < MAX_NUMBERS; ++i) {
        SDL_DestroyTexture(Cell::sTextureOfNumbers[i]);
        Cell::sTextureOfNumbers[i] = nullptr;
    }
}

void Minesweeper::play() {
    //Encompeses all loops
    while (mGameState != EXIT) {
        switch (mGameState) {
            case MENU:
                menuLoop();
            case BOARD:
                boardLoop();
        }	
    }
}

Minesweeper::~Minesweeper() {
    //Destroy window and renderer
    SDL_DestroyWindow(mWindow);
    SDL_DestroyRenderer(mRenderer);
    mWindow = nullptr;
    mRenderer = nullptr;

    //Quit SDL and TTF
    SDL_Quit();
    TTF_Quit();
}