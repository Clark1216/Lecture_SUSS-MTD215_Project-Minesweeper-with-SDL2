#include "board.h"

Board::Board() :
    mState(FIRSTCELL), mMaxRows(14), mMaxCols(18), mBombs(40) {

}

void Board::loadTextures(SDL_Renderer* renderer) {
	//load font
	Cell::sFont = getFont(Cell::sFONT_SIZE);

	//Load textures for all 9 numbers (0-8)
	//0 Does not have a texture
	Cell::sTextureOfNumbers[0] = nullptr;
	for (int i = 1; i < 9; ++i) {
		Cell::sTextureOfNumbers[i] = loadTexture(renderer, Cell::sFont, Cell::sCOLOUR_OF_NUMBERS[i], std::to_string(i).c_str());
	}

	//Load flag and bomb texture
	Cell::sFlagTexture = loadTexture(renderer, "assets/flag.bmp");
	Cell::sBombTexture = loadTexture(renderer, "assets/bomb.bmp");

}

void Board::create(const int maxRows, const int maxCols, const int bombs) {
	mMaxRows = maxRows;
	mMaxCols = maxCols;
	mBombs = bombs;

	//Starting coordinates
	int x = Cell::sGAP;
	int y = Cell::sGAP;
	//Create cells
	for (int row = 0; row < mMaxRows; ++row) {
		for (int col = 0; col < mMaxCols; ++col) {
			SDL_Rect rect = {x, y, Cell::sWIDTH, Cell::sHEIGHT};
			Cell cell(rect, Cell::sCOLOUR);
			mBoard.push_back(cell);
			x += Cell::sWIDTH + Cell::sGAP;
		}
		x = Cell::sGAP;
		y += Cell::sHEIGHT + Cell::sGAP;
	}
}

//Get index of 1d array from 2d arguments
inline int Board::getIndex(const int row, const int col) {
    return row * mMaxCols + col; 
}

//Return true if valid index (might not be necessary when using std::vector)
inline bool Board::validIndex(const int row, const int col) {
	return row > -1 && col > -1 && row < mMaxRows && col < mMaxCols;
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
	for (int row = 0; row < mMaxRows; ++row) {
		for (int col = 0; col < mMaxCols; ++col) {
			func(row, col);
		}
	}
}

//Generate the bombs (mines) on the board on the first click and don't place any bombs within a 3 x 3 area of the clicked cell
void Board::generateBombs(std::vector<Cell>& board, const int firstClickedRow, const int firstClickedCol) {
	/*--------------------------------Plant random bombs----------------------------*/
	//For our medium size mode we will use 40 bombs
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
		board[randomIndex].plantBomb();
	}

	/*-----------------------------------Plant numbers------------------------------*/
	//Plant numbers in cells based on how many bombs there are in an 3 x 3 area
	//Plant number for each cell
	forEachCell([&](const int row, const int col) {
		int index = getIndex(row, col);
		if (!board[index].bombPlanted()) {
			//Search bombs in a 3 x 3 area
			int bombsFound = 0;
			forEachNeighbour([&](const int tempRow, const int tempCol) {
				int tempIndex = getIndex(tempRow, tempCol);
				if (board[tempIndex].bombPlanted()) {
					bombsFound++;
				}
			}, row, col);
			board[index].plantNumber(bombsFound);
		}
	});
}

//Recursive function to open (expand) board
void Board::openBoard(std::vector<Cell>& board, const int row, const int col) {
	//For each neighbouring cell
	forEachNeighbour([&](const int tempRow, const int tempCol) {
		int tempIndex = getIndex(tempRow, tempCol);
		Cell& tempCell = board[tempIndex];
		if (!tempCell.isOpen()) {
			if (tempCell.numberPlanted()) {
				tempCell.open();
			} else if (tempCell.bombPlanted()) {
				return;
			} else {
				tempCell.open();
				openBoard(board, tempRow, tempCol);
			}
		} 
	}, row, col);
}

void Board::handleMouseDown(const SDL_Event& event, bool& renderFlag) {
    forEachCell([&](const int row, const int col){
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        Cell& cell = mBoard[getIndex(row, col)];
        if (cell.isMouseInside(mouseX, mouseY)) {
            //Handle left mouse click
            if (event.button.button == SDL_BUTTON_LEFT) {
                cell.open();
                if (mState == PLAYING) {
                    if (cell.bombPlanted()) {
                        mState = LOSE;
                    } else if (!cell.numberPlanted()) {
                        //Open neighboring cells if they do not have bombs or numbers
                        openBoard(mBoard, row, col);
                    }
                } else if (mState == FIRSTCELL) {
                    //Generate random bombs
                    generateBombs(mBoard, row, col);
                    //Open neighboring cells
                    openBoard(mBoard, row, col);
                    mState = PLAYING;
                }
                renderFlag = true;
            } 
            //Handle right mouse click
            else if (event.button.button == SDL_BUTTON_RIGHT) {
                cell.setFlag();
                renderFlag = true;
            }
        }
    });
}

void Board::handleState() {
	if (mState == LOSE) {
		std::cout << "BOMB FOUND: YOU HAVE LOST!" << std::endl;
		mState = PLAYING;
	}
}

void Board::render(SDL_Renderer* renderer) {
    //Render cells
    for (Cell& cell : mBoard) {
        cell.render(renderer);
    }
}

Board::~Board() {
	//Free textures
	for (SDL_Texture* texture : Cell::sTextureOfNumbers) {
		SDL_DestroyTexture(texture);
		texture = nullptr;
	}
	SDL_DestroyTexture(Cell::sFlagTexture);
	SDL_DestroyTexture(Cell::sBombTexture);
	Cell::sFlagTexture = nullptr;
	Cell::sBombTexture = nullptr;

	//Close font
	TTF_CloseFont(Cell::sFont);
	Cell::sFont = nullptr;
}