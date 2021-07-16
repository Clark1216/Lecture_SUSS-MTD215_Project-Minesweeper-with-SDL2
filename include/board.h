#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <algorithm>
#include <random>
#include <chrono>

#include "cell.h"
#include "loadTexture.h"

//Track game state
//(FIRSTCELL state is when bombs are generated - this guarantees no bombs are generated in a 3 x 3 area on the first cell clicked)
enum GameState {MENU, FIRSTCELL, PLAYING, LOSE, WIN};

//Board filled with cells
class Board {
    private:
        int mMaxRows;
        int mMaxCols;

        std::vector<Cell> mBoard;

    public:
        Board(SDL_Renderer* renderer);

        inline int getIndex(const int row, const int col);
        inline bool validIndex(const int row, const int col);

        void forEachNeighbour(const std::function<void(const int, const int)>& func, const int row, const int col);
        void forEachCell(const std::function<void(const int, const int)>& func);

        void generateBombs(std::vector<Cell>& board, const int firstClickedRow, const int firstClickedCol);
        void openBoard(std::vector<Cell>& board, const int row, const int col);

        void handleMouseDown(const SDL_Event& event, bool& renderFlag, GameState& gameState);
        void render(SDL_Renderer* renderer);

};