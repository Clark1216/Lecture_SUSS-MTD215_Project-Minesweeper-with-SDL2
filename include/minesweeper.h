/*Singleton to encapsulate our minesweepr game*/
#pragma once
#include <string>
#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>

#include "boardDetails.h"
#include "menu.h"
#include "HUD.h"
#include "board.h"

enum GameState {MENU, BOARD, RESET, EXIT};

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