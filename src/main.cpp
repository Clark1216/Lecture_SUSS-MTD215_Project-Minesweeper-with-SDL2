/*A Minesweeper made made using SDL and SDL_ttf*/
#include <iostream>
#include <string>

#include <SDL.h>
#include <SDL_ttf.h>

#include "board.h"

//SDL requires c style paramaters in the main function
int main( int argc, char* args[] ) {
	/*------------------------------------Initialise--------------------------------*/
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
	SDL_Window* window = SDL_CreateWindow("Minesweeper", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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

	/*------------------------------------Game Loop---------------------------------*/
	//Create board
	Board board(renderer);

	//Set loop variables
	GameState gameState = FIRSTCELL;
	bool exit = false;
	SDL_Event event;
	bool renderFlag= true;

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
				board.handleMouseDown(event, renderFlag, gameState);
			}
		}
		
		if (gameState == LOSE) {
			std::cout << "BOMB FOUND: YOU HAVE LOST!" << std::endl;
			gameState = PLAYING;
		}

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

		//Slow down program
		SDL_Delay(20);
	}

	/*--------------------------------Ending Programme--------------------------------*/
	//Free textures
	for (SDL_Texture* texture : Cell::sTextureOfNumbers) {
		SDL_DestroyTexture(texture);
	}
	SDL_DestroyTexture(Cell::sFlagTexture);
	SDL_DestroyTexture(Cell::sBombTexture);

	//Close font
	TTF_CloseFont(Cell::sFont);
	Cell::sFont = nullptr;

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