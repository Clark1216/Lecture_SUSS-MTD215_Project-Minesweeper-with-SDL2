/*This is an application made using SDL and SDL_ttf*/
#include <iostream>
#include <string>
#include <vector>

#include <SDL.h>
#include <SDL_ttf.h>

#include "loadTexture.h"
#include "UI_element.h"
#include "button.h"
#include "cell.h"

//Get index of 1d array from 2d arguments
static inline int getIndex(int row, int col, int maxCols) {
	return row * maxCols + col; 
}

//SDL requires c style paramaters in the main function
int main( int argc, char* args[] ) {
	//Screen dimension
	const int SCREEN_WIDTH = 706;
	const int SCREEN_HEIGHT = 550;

	//Initialise SDL
	if(SDL_Init(SDL_INIT_VIDEO) < 0 ) {
		std::cout << "SDL could not initialise! SDL_Error: " << SDL_GetError() << std::endl;
		return -1;
	}

	//Initialise SDL_ttf
	if (TTF_Init() < 0) {
		std::cout << "SDL_ttf could not initialise! Error: " << TTF_GetError() << std::endl;
		return -1;
	}

	//Create window
	SDL_Window* window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == NULL) {
		std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		return -1;
	}

	//Create renderer
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
	if (renderer == NULL) {
		std::cout << "SDL could not create renderer! Error: " << SDL_GetError() << std::endl;
		return -1;
	}

	//Load font
	const int fontSize = 30;
	SDL_Color fontColour = {255, 255, 255, 255}; //Black
	TTF_Font* font = TTF_OpenFont("assets/octin sports free.ttf", fontSize);
	if (font == NULL) {
		std::cout << "Failed to load font! Error: " << TTF_GetError() << std::endl;
		return -1;
	}

	/*------------------------------Create board of cells---------------------------*/
	//Cell properties
	const int MAX_ROWS = 14;
	const int MAX_COLS = 18;
	const int CELL_GAP = 4;
	const int CELL_WIDTH = 35;
	const int CELL_HEIGHT = CELL_WIDTH;
	const SDL_Color CELL_COLOUR = {158, 158, 158, 255}; //DARK GREY
	const SDL_Color PRESSED_CELL_COLOUR = {209, 209, 209, 255}; //LIGHT GREY

	//Load cell font
	const int CELL_FONT_SIZE = 30;
	TTF_Font* CELL_FONT = TTF_OpenFont("assets/octin sports free.ttf", CELL_FONT_SIZE);
	if (font == NULL) {
		std::cout << "Failed to load cell font! Error: " << TTF_GetError() << std::endl;
		return -1;
	}

	//Define colours for all 9 numbers (0-8)
	const SDL_Color colourOfNumbers[9] = 
	{{  0,   0,   0,   0},  //0 = Nothing (wont be used but kept to make index make more sense)
	 { 20,  57, 168, 255},  //1 = BLUE
	 { 20, 148,  18, 255},	//2 = GREEN
	 {179,  30,  30, 255},  //3 = RED
	 {103,  28, 173, 255},  //4 = PURPLE
	 {133,  20,  20, 255},  //5 = MAROON
	 {  9, 179, 164, 255},  //6 = TURQUOISE
	 {  0,   0,   0, 255},  //7 = BLACK
	 {255, 255, 255, 255}}; //8 = WHITE

	//Load textures for all 9 numbers (0-8)
	SDL_Texture* textureOfNumbers[9];
	//0 Does not have a texture
	textureOfNumbers[0] = nullptr;
	for (int i = 1; i < 9; ++i) {
		textureOfNumbers[i] = loadTexture(renderer, CELL_FONT, colourOfNumbers[i], std::to_string(i).c_str());
	}
	//Load flag and bomb texture
	SDL_Texture* flagTexture = loadTexture(renderer, "assets/flag.bmp");
	SDL_Texture* bombTexture = loadTexture(renderer, "assets/bomb.bmp");

	//Set colours and textures in Cell class
	Cell::sCellColour = CELL_COLOUR;
	Cell::sPressedCellColour = PRESSED_CELL_COLOUR;
	Cell::sTextureOfNumbers = textureOfNumbers;
	Cell::sFlagTexture = flagTexture;
	Cell::sBombTexture = bombTexture;

	//Starting coordinates
	int x = CELL_GAP;
	int y = CELL_GAP;
	//Create cells
	std::vector<Cell> board;
	for (int row = 0; row < MAX_ROWS; ++row) {
		for (int col = 0; col < MAX_COLS; ++col) {
			SDL_Rect rect = {x, y, CELL_WIDTH, CELL_HEIGHT};
			Cell cell(rect, CELL_COLOUR);
			board.push_back(cell);
			x += CELL_WIDTH + CELL_GAP;
		}
		x = CELL_GAP;
		y += CELL_HEIGHT + CELL_GAP;
	}

	/*------------------------------Generate random bombs---------------------------*/
	//For our medium size mode we will use

	/*------------------------------------Game Loop---------------------------------*/
	//Set loop variables
	bool exit = false;
	SDL_Event event;

	//Main game loop
	while (!exit) {
		//Handle events
		while (SDL_PollEvent(&event) != 0) {
			//Handle quitting
			if (event.type == SDL_QUIT) {
				exit = true;
			}
			//Handle mouse click
			if (event.type == SDL_MOUSEBUTTONDOWN) {

			}
		}

		//Clear screen
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		//Render
		for (int row = 0; row < MAX_ROWS; ++row) {
			for (int col = 0; col < MAX_COLS; ++col) {
				int index = getIndex(row, col, MAX_COLS);
				board[index].render(renderer);
			}
		}

		//Update screen from backbuffer and clear backbuffer
		SDL_RenderPresent(renderer);

		//Slow down program
		SDL_Delay(20);
	}

	/*--------------------------------Ending Programme--------------------------------*/
	//Free textures
	for (int i = 0; i < 10; ++i) {
		SDL_DestroyTexture(textureOfNumbers[i]);
	}
	SDL_DestroyTexture(flagTexture);
	SDL_DestroyTexture(bombTexture);

	//Destroy window and renderer
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	window = nullptr;
	window = nullptr;

	//Quit SDL and TTF
	SDL_Quit();
	TTF_Quit();

	//Exit program
	std::cout << "Exiting program..." << std::endl;
	return 0;
}