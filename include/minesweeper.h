/*Singleton to encapsulate our minesweepr game*/
#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <string>
#include "loadTexture.h"
#include "boardDetails.h"
#include "cell.h"
#include "menu.h"
#include "HUD.h"
#include "board.h"

enum GameState {MENU, BOARD, EXIT};

class Minesweeper {
    private:
        SDL_Window* mWindow;
		SDL_Renderer* mRenderer;
		GameState mGameState;
		BoardDetails mBoardDetails;

        void menuLoop();
        void boardLoop();

    public:
        Minesweeper();
        void play();
        ~Minesweeper();

};
