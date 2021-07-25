#include "board.h"

Board::Board(const BoardDetails& boardDetails, const int START_X, const int START_Y, const int CELL_WIDTH, const int CELL_HEIGHT, const int CELL_GAP) 
	: mRows(boardDetails.rows), mCols(boardDetails.cols), mBombs(boardDetails.bombs), mState(FIRST_CELL), mBoard(nullptr) {

	//Create array on the heap
	mBoard = new Cell[mRows * mCols];
	
	//Starting coordinates
	int x = START_X + CELL_GAP;
	int y = START_Y + CELL_GAP;
	//Create cells
	for (int row = 0; row < mRows; ++row) {
		for (int col = 0; col < mCols; ++col) {
			SDL_Rect rect = {x, y, CELL_WIDTH, CELL_HEIGHT};
			mBoard[getIndex(row, col)] = Cell(rect);
			x += CELL_WIDTH + CELL_GAP;
		}
		x = CELL_GAP;
		y += CELL_HEIGHT + CELL_GAP;
	}
}


//Get index of 1d array from 2d arguments
inline int Board::getIndex(const int row, const int col) {
    return row * mCols + col; 
}

//Return true if valid index (might not be necessary when using std::vector)
inline bool Board::validIndex(const int row, const int col) {
	return row > -1 && col > -1 && row < mRows && col < mCols;
}

//For each neighbour around the chosen cell, do something
void Board::forEachNeighbour(const std::function<void(const int, const int)>& func, const int row, const int col) {
	for (int deltaRow = -1; deltaRow < 2; ++deltaRow) {
		for (int deltaCol = -1; deltaCol < 2; ++deltaCol) {
			int tempRow = row + deltaRow;
			int tempCol = col + deltaCol;
			if (validIndex(tempRow, tempCol)) {
				func(tempRow, tempCol);
			}
		}
	}
}

//For each cell on the board, do something to it using the row and col
void Board::forEachCell(const std::function<void(const int, const int)>& func) {
	//Loop throw rows and cols of board
	for (int row = 0; row < mRows; ++row) {
		for (int col = 0; col < mCols; ++col) {
			func(row, col);
		}
	}
}

//Generate the bombs (mines) on the board on the first click and don't place any bombs within a 3 x 3 area of the clicked cell
void Board::generateBombs(const int firstClickedRow, const int firstClickedCol) {
	/*--------------------------------Plant random bombs----------------------------*/
	//Create array with same size as board and hold the index in each element
	const int NUMBER_OF_BOMBS = mBombs;
	std::vector<int> bombGeneratorArray;
	
	//Create lambda to add to generator array
	auto addCellIndex = [&](const int row, const int col) {
		//Ignore 3 x 3 area around the first cell clicked (no bombs in this range)
		if ((row < firstClickedRow - 1) || (row > firstClickedRow + 1) || (col < firstClickedCol - 1) || (col > firstClickedCol + 1)) {
			int index = getIndex(row, col);
			bombGeneratorArray.push_back(index);
		} 
	};

	//Push back cell index onto generator array
	forEachCell(addCellIndex);

	//Obtain a time-based seed
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	//Shuffle bomb array
	std::shuffle(bombGeneratorArray.begin(), bombGeneratorArray.end(), std::default_random_engine(seed));

	//Use shuffled bomb array to plant bombs on the board
	for (int i = 0; i < NUMBER_OF_BOMBS; ++i) {
		int randomIndex = bombGeneratorArray[i];
		mBoard[randomIndex].plantBomb();
	}

	/*-----------------------------------Plant numbers------------------------------*/
	//Plant numbers in cells based on how many bombs there are in an 3 x 3 area
	//Plant number for each cell
	forEachCell([&](const int row, const int col) {
		int index = getIndex(row, col);
		if (!mBoard[index].bombPlanted()) {
			//Search bombs in a 3 x 3 area
			int bombsFound = 0;
			forEachNeighbour([&](const int tempRow, const int tempCol) {
				int tempIndex = getIndex(tempRow, tempCol);
				if (mBoard[tempIndex].bombPlanted()) {
					bombsFound++;
				}
			}, row, col);
			mBoard[index].plantNumber(bombsFound);
		}
	});
}

//Recursive function to open (expand) board
//Need to pass in hud to remove any flags in the way
void Board::openBoard(const int row, const int col, HUD& hud) {
	//For each neighbouring cell
	forEachNeighbour([&](const int tempRow, const int tempCol) {
		int tempIndex = getIndex(tempRow, tempCol);
		Cell& tempCell = mBoard[tempIndex];
		if (!tempCell.isOpen()) {
			if (tempCell.bombPlanted())
				return;
			else {
				tempCell.open();
				if (tempCell.mFlag) {
					//Remove flag
					tempCell.setFlag();
					hud.mFlagCounter.incrementCounter();
				}
				if (!tempCell.numberPlanted()) {
					openBoard(tempRow, tempCol, hud);
				}
			}
		} 
	}, row, col);
}

void Board::handleMouseDown(const SDL_Event& event, HUD& hud, bool& render) {
    forEachCell([&](const int row, const int col){
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        Cell& cell = mBoard[getIndex(row, col)];
        if (cell.isMouseInside(mouseX, mouseY)) {
            //Handle left mouse click
            if (event.button.button == SDL_BUTTON_LEFT && !cell.mFlag) {
                cell.open();
                if (mState == PLAYING) {
                    if (cell.bombPlanted()) {
                        mState = LOSE;
                    } else if (!cell.numberPlanted()) {
                        //Open neighboring cells if they do not have bombs or numbers
                        openBoard(row, col, hud);
                    }
                } else if (mState == FIRST_CELL) {
                    //Generate random bombs
                    generateBombs(row, col);
                    //Open neighboring cells
                    openBoard(row, col, hud);
                    mState = PLAYING;
                }
                render = true;
				return;
            } 
            //Handle right mouse click
            else if (event.button.button == SDL_BUTTON_RIGHT) {
				if (!cell.isOpen()) {
					//If counter is greater than 0...
					//... or if counter is equal to 0 and a flag is set
					if (hud.mFlagCounter.getCounter() > 0 || hud.mFlagCounter.getCounter() == 0 && cell.mFlag) {
						//If flag was set, decrement flag count, else increment flag count#
						cell.setFlag() ? hud.mFlagCounter.decrementCounter() : hud.mFlagCounter.incrementCounter();
						
						render = true;
						return;
					}
				}
            }
        }
    });
}

void Board::render(SDL_Renderer* renderer) {
    //Render 
	forEachCell([&](const int row, const int col){
		mBoard[getIndex(row,col)].render(renderer);
	});
}

Board::~Board() {
	delete[] mBoard;
}