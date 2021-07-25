#include "minesweeper.h"

Minesweeper::Minesweeper() 
: mWindow(nullptr), mRenderer(nullptr), mGameState(MENU), mBoardDetails({0, 0, 0}) {
    //Initial screen dimensions to be equal to menu dimensions
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

    //Define button dimensions, gap, and colour
    const int BUTTON_GAP = 10;
    const int BUTTON_WIDTH = 153;
    const int BUTTON_HEIGHT = 130;
    const SDL_Color BUTTON_COLOUR = {114, 166, 176, 255}; //Grey-ish blue

    //Define max rows and max columns for each difficulty and bombs
    //BoardDetails{maxRows, maxCols, bombs}
    BoardDetails easyDetails = {9, 9, 10};
    BoardDetails mediumDetails = {16, 16, 40};
    BoardDetails hardDetails = {16, 30, 99};
    BoardDetails allBoardDetails[MENU_BUTTON_COUNT] = {easyDetails, mediumDetails, hardDetails};

    //load font
    const int FONT_SIZE = 30;
    SDL_Color FONT_COLOUR = {255, 255, 255, 255}; //White
	TTF_Font* font = loadFont(FONT_SIZE);

    //Load texture for each button based on the difficulty they represent
    const char* difficulties[MENU_BUTTON_COUNT] = {"Easy", "Medium", "Hard"};
    SDL_Texture* menuTextures[MENU_BUTTON_COUNT];
    for (int i = 0; i < MENU_BUTTON_COUNT; ++i) {
        menuTextures[i] = loadTexture(mRenderer, font, FONT_COLOUR, difficulties[i]);
    }

    //Close font
    TTF_CloseFont(font);
    font = nullptr;

    //Create menu
    Menu menu(allBoardDetails, menuTextures, BUTTON_GAP, BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_COLOUR);

    //Set loop variables
    SDL_Event event;
    bool updateFlag = false;
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
                menu.handleMouseDown(event, updateFlag);
            }
        }

        //If a button was clicked, update flag was set to true
        if (updateFlag) {
            //Change the board details based on which button was clicked
            if (menu.handleState(mBoardDetails)) {
                //Change game state to board
                mGameState = BOARD;
            }
            updateFlag = false;
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

    //Free textures
    for (int i = 0; i < MENU_BUTTON_COUNT; ++i) {
        SDL_DestroyTexture(menuTextures[i]);
        menuTextures[i] = nullptr;
    }

}

//Board loop contains the game difficulty chosen with hud
void Minesweeper::boardLoop() {
/*-----------------------------Board sizing is based on difficulty and HUD-----------------------*/
    //Define size of cells
    const int CELL_WIDTH = 35;
    const int CELL_HEIGHT = 35;
    const int CELL_GAP = 4;

    //Extract board details
    const int ROWS = mBoardDetails.rows;
    const int COLS = mBoardDetails.cols;
    const int STARTING_FLAG_COUNT = mBoardDetails.bombs;

    //Define HUD height and gap
    const int HUD_HEIGHT = 50;
    const int HUD_GAP = 4;

    //Change size of window based on board dimensions, size of hud, and size of cells
    const int SCREEN_WIDTH = CELL_WIDTH * COLS + CELL_GAP * (COLS + 1);
    const int SCREEN_HEIGHT = CELL_HEIGHT * ROWS + CELL_GAP * (ROWS + 1) + HUD_HEIGHT + 2 * CELL_GAP;
    SDL_SetWindowSize(mWindow, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_SetWindowPosition(mWindow, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

/*--------------------------------------------Create HUD------------------------------------------*/
    //HUD contians menu button, timer, flag counter, and reset button 
    //Define starting flag count based on number of bombs/mines placed

    //Load Texture for menu and reset button
    //Load font
    int font_size = 25;
    SDL_Color FONT_COLOUR = {255, 255, 255, 255}; //White
    TTF_Font* font = loadFont(font_size);

    //Load texture for each button based on the difficulty they represent
    SDL_Texture* menuButtonTexture = loadTexture(mRenderer, font, FONT_COLOUR, "Menu");
    SDL_Texture* resetButtonTexture = loadTexture(mRenderer, font, FONT_COLOUR, "Reset");

    //Close font
    TTF_CloseFont(font);
    font = nullptr;

    //Create hud
    HUD hud(mRenderer, menuButtonTexture, resetButtonTexture, SCREEN_WIDTH, HUD_HEIGHT, HUD_GAP, STARTING_FLAG_COUNT); 

/*-------------------------------------------Create board-----------------------------------------*/
    //load font
    font_size = 30;
    font = loadFont(font_size);

    //Define colours for all 9 numbers (0-8) (for the board)
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

    //Close font
    TTF_CloseFont(font);
    font = nullptr;

    //Set Cell flag and bomb textures
    Cell::sFlagTexture = loadTexture(mRenderer, "assets/flag.bmp");
    Cell::sBombTexture = loadTexture(mRenderer, "assets/bomb.bmp");

    //Set Cell colours
    Cell::sCOLOUR = {158, 158, 158, 255}; //DARK GREY
    Cell::sPRESSED_COLOUR = {209, 209, 209, 255}; //LIGHT GREY
    
    //Start board coordinates below HUD
    const int START_X = 0;
    const int START_Y = HUD_HEIGHT + CELL_GAP;

    //Create board
    Board board(mBoardDetails, START_X, START_Y, CELL_WIDTH, CELL_HEIGHT, CELL_GAP);
    
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
                board.handleMouseDown(event, hud, renderFlag);
                hud.handleMouseDown(event);
            }
        }

        if (renderFlag) {
            //Clear screen
            SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
            SDL_RenderClear(mRenderer);

            //Render HUD
            hud.render(mRenderer);

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

    //Destroy textures
    SDL_DestroyTexture(Cell::sFlagTexture);
    SDL_DestroyTexture(Cell::sBombTexture);
    SDL_DestroyTexture(menuButtonTexture);
    SDL_DestroyTexture(resetButtonTexture);
    Cell::sFlagTexture = nullptr;
    Cell::sBombTexture = nullptr;
    menuButtonTexture = nullptr;
    resetButtonTexture = nullptr;

    for (auto& texture : Cell::sTextureOfNumbers) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
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