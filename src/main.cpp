/*A Minesweeper made made using SDL and SDL_ttf*/
#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include "menu.h"
#include "cell.h"
#include "board.h"

enum GameState {MENU, BOARD, EXIT};

//Menu loop for choosing difficulty
void menuLoop(SDL_Window* window, SDL_Renderer* renderer, BoardDetails& boardDetails, GameState& gameState) {
	//Change size of window
	const int SCREEN_WIDTH = 500;
	const int SCREEN_HEIGHT = 150;
	SDL_SetWindowSize(window, SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

	//Create menu
	Menu menu;
	menu.createButtons(renderer);

	/*------------------------------------Menu Loop---------------------------------*/
	//Set loop variables
	SDL_Event event;
	bool renderFlag = true;
	bool buttonClicked = false;

	//Menu loop
	while (gameState == MENU) {
		//Handle events
		while (SDL_PollEvent(&event) != 0) {
			//Handle quitting
			if (event.type == SDL_QUIT) {
				gameState = EXIT;
			}
			//Handle mouse click
			if (event.type == SDL_MOUSEBUTTONDOWN) {
				menu.handleMouseDown(event, renderFlag);
			}
		}

		//Handle state and return true if button was clicked
		buttonClicked = menu.handleState(boardDetails);
		if (buttonClicked) {
			gameState = BOARD;
		}

		if (renderFlag) {
			//Clear screen
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderClear(renderer);

			//Render board
			menu.render(renderer);

			//Update screen from backbuffer and clear backbuffer
			SDL_RenderPresent(renderer);

			//Reset render flag
			renderFlag = false;
		}

		//Slow down programme
		SDL_Delay(20);
	}

}

//Board loop contianing the chosen difficulty of the game
void boardLoop(SDL_Window* window, SDL_Renderer* renderer, const BoardDetails& boardDetails, GameState& gameState) {
	//Change size of window
	const int MAX_ROWS = boardDetails.mMaxRows;
	const int MAX_COLS = boardDetails.mMaxCols;
	const int BOMBS = boardDetails.mBombs;

	const int SCREEN_WIDTH = Cell::sWIDTH * MAX_COLS + Cell::sGAP * (MAX_COLS + 1);
	const int SCREEN_HEIGHT = Cell::sHEIGHT * MAX_ROWS + Cell::sGAP * (MAX_ROWS + 1);
	SDL_SetWindowSize(window, SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

	/*----------------------------------Create buttons------------------------------*/

	/*-----------------------------------Create board-------------------------------*/
	Board board;
	board.loadTextures(renderer);
	board.create(MAX_ROWS, MAX_COLS, BOMBS);
	
	/*------------------------------------Board Loop---------------------------------*/
	//Set loop variables
	SDL_Event event;
	bool renderFlag= true;

	//Board loop
	while (gameState == BOARD) {
		//Handle events
		while (SDL_PollEvent(&event) != 0) {
			//Handle quitting
			if (event.type == SDL_QUIT) {
				gameState = EXIT;
			}
			//Handle mouse click
			if (event.type == SDL_MOUSEBUTTONDOWN) {
				board.handleMouseDown(event, renderFlag);
			}
		}
		
		//Handle current state of board
		board.handleState();

		if (renderFlag) {
			//Clear screen
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderClear(renderer);

			//Render board
			board.render(renderer);

			//Update screen from backbuffer and clear backbuffer
			SDL_RenderPresent(renderer);

			//Reset render flag
			renderFlag = false;
		}

		//Slow down programme
		SDL_Delay(20);
	}

}  

//Encompeses all loops
void gameLoop(SDL_Window* window, SDL_Renderer* renderer) {

	GameState gameState = MENU;
	BoardDetails boardDetails;

	while (gameState != EXIT) {
		if (gameState == MENU) {
			menuLoop(window, renderer, boardDetails, gameState);
		} else if (gameState == BOARD) {
			boardLoop(window, renderer, boardDetails, gameState);
		}
	}
}

//SDL requires c style paramaters in the main function
int main( int argc, char* args[] ) {
	/*------------------------------------Initialise--------------------------------*/
	//Screen dimension
	const int SCREEN_WIDTH = 500;
	const int SCREEN_HEIGHT = 150;

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
	SDL_Window* window = SDL_CreateWindow("Minesweeper", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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

	/*------------------------------------Game Loop-----------------------------------*/
	gameLoop(window, renderer);

	/*--------------------------------Ending Programme--------------------------------*/
	//Destroy window and renderer
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	window = nullptr;
	renderer = nullptr;

	//Quit SDL and TTF
	SDL_Quit();
	TTF_Quit();

	//Exit program
	std::cout << "Exiting program..." << std::endl;
	return 0;
}