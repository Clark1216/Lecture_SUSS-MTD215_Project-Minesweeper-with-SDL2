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

    //Create menu
    Menu menu(mRenderer, SCREEN_WIDTH, SCREEN_HEIGHT);

    //Create lambda to handle difficulty chosen
    auto handleDifficulty = [&](const BoardDetails& boardDetails){
        mBoardDetails = boardDetails;
        mGameState = BOARD;
    };

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
                menu.handleMouseDown(event, handleDifficulty);
            }
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

}

//Board loop contains the game difficulty chosen with hud
void Minesweeper::boardLoop() {
    //If the game state was just reseted then set it to board state
    if (mGameState == RESET)
        mGameState = BOARD;
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

    //Create hud
    HUD hud(mRenderer, SCREEN_WIDTH, HUD_HEIGHT, HUD_GAP, STARTING_FLAG_COUNT); 

/*-------------------------------------------Create board-----------------------------------------*/
    //Start board coordinates below HUD
    const int START_X = 0;
    const int START_Y = HUD_HEIGHT + CELL_GAP;

    //Create board
    Board board(mRenderer, mBoardDetails, START_X, START_Y, CELL_WIDTH, CELL_HEIGHT, CELL_GAP);
    
    //Create lamda for handling menu event
    auto handleMenuEvent = [&](){
        mGameState = MENU;
    };

    //Create lambda for handling reset event
    auto handleResetEvent = [&](){
        mGameState = RESET;
    };

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
                //Pass hud down for any changes in flag count
                board.handleMouseDown(event, hud, renderFlag);
                hud.handleMouseDown(event, handleMenuEvent, handleResetEvent);
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
}

void Minesweeper::play() {
    //Encompeses all loops
    while (mGameState != EXIT) {
        switch (mGameState) {
            case MENU:
                menuLoop();
                break;
            case BOARD:
            case RESET:
                boardLoop();
                break;
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