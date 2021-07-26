#include "board.h"

Board::Board(SDL_Renderer* renderer, const BoardDetails& boardDetails, const int START_X, const int START_Y, const int CELL_WIDTH, const int CELL_HEIGHT, const int CELL_GAP) 
	: mRows(boardDetails.rows), mCols(boardDetails.cols), mBombs(boardDetails.bombs), 
	mCellsOpened(0), mCellsToOpen(boardDetails.rows * boardDetails.cols - boardDetails.bombs),
	mState(FIRST_CELL), mBoard(nullptr) {

	//load font
    const int FONT_SIZE = 30;
    TTF_Font* font = loadFont(FONT_SIZE);

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
        Cell::sTextureOfNumbers[i] = loadTexture(renderer, font, COLOUR_OF_NUMBERS[i], std::to_string(i).c_str());
    }

    //Close font
    TTF_CloseFont(font);
    font = nullptr;

    //Set Cell flag and bomb textures
    Cell::sFlagTexture = loadTexture(renderer, "assets/flag.bmp");
    Cell::sBombTexture = loadTexture(renderer, "assets/bomb.bmp");

    //Set Cell colours
    Cell::sCOLOUR = {158, 158, 158, 255}; //DARK GREY
    Cell::sPRESSED_COLOUR = {209, 209, 209, 255}; //LIGHT GREY
	Cell::sMARK_COLOUR = {232, 16, 5, 255}; //RED
	
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
//Need to pass in hud to remove any flags in the way that will cause hud to update
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
				mCellsOpened++;
				if (tempCell.getFlag()) {
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
		if (mState != LOSE && mState != WIN) {
			int mouseX, mouseY;
			SDL_GetMouseState(&mouseX, &mouseY);
			Cell& cell = mBoard[getIndex(row, col)];
			if (cell.isMouseInside(mouseX, mouseY)) {
				//Handle left mouse click
				if (event.button.button == SDL_BUTTON_LEFT && !cell.getFlag()) {
					cell.open();
					mCellsOpened++;
					if (mState == PLAYING) {
						if (cell.bombPlanted()) {
							//Mark current cell as red
							mState = LOSE;
							cell.mark();
							revealBombs();
							hud.mTimer.stop("RED");
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

					//Winning condition
					if (mCellsOpened == mCellsToOpen && mState != LOSE) {
						mState = WIN;
						hud.mTimer.stop("GREEN");
					}

					render = true;
					return;
				} 
				//Handle right mouse click
				else if (event.button.button == SDL_BUTTON_RIGHT) {
					if (!cell.isOpen()) {
						//If counter is greater than 0...
						//... or if counter is equal to 0 and a flag is set
						if (hud.mFlagCounter.getCounter() > 0 || hud.mFlagCounter.getCounter() == 0 && cell.getFlag()) {
							//If flag was set, decrement flag count, else increment flag count#
							cell.setFlag() ? hud.mFlagCounter.decrementCounter() : hud.mFlagCounter.incrementCounter();
							
							render = true;
							return;
						}
					}
				}
			}
		}
    });
}

void Board::revealBombs() {
	forEachCell([&](const int row, const int col) {
		Cell& cell = mBoard[getIndex(row, col)];
		if (cell.bombPlanted() && !cell.isOpen()) {
			cell.open();
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
	//Delete board
	delete[] mBoard;

	//Destroy textures
    SDL_DestroyTexture(Cell::sFlagTexture);
    SDL_DestroyTexture(Cell::sBombTexture);
    Cell::sFlagTexture = nullptr;
    Cell::sBombTexture = nullptr;

    for (auto& texture : Cell::sTextureOfNumbers) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
}