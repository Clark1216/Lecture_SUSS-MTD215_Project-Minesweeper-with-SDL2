#pragma once
#include <SDL.h>
#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>
#include <random>
#include <chrono>

#include "boardDetails.h"
#include "cell.h"

//Track game state
//(FIRSTCELL state is when bombs are generated - this guarantees no bombs are generated in a 3 x 3 area on the first cell clicked)
enum BoardState {FIRST_CELL, PLAYING, LOSE, WIN};

//Board filled with cells
class Board {
    private:
        int mRows;
        int mCols;
        int mBombs;
        
        BoardState mState;
        Cell* mBoard;

        inline int getIndex(const int row, const int col);
        inline bool validIndex(const int row, const int col);

        void forEachNeighbour(const std::function<void(const int, const int)>& func, const int row, const int col);
        void forEachCell(const std::function<void(const int, const int)>& func);

        void generateBombs(const int firstClickedRow, const int firstClickedCol);
        void openBoard(const int row, const int col);

    public:
        Board(const BoardDetails& boardDetails, const int CELL_WIDTH, const int CELL_HEIGHT, const int CELL_GAP);
        void handleMouseDown(const SDL_Event& event, bool& renderFlag);
        void handleState();
        void render(SDL_Renderer* renderer);
        ~Board();

};