/*This is an application made using SDL and SDL_ttf*/

//Using SDL and standard IO
#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include "UI_element.h"

int main( int argc, char* args[] ) {
	//Screen dimension constants
	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 480;

	//Initialise SDL
	if(SDL_Init(SDL_INIT_VIDEO) < 0 ) {
		std::cout << "SDL could not initialise! SDL_Error: " << SDL_GetError();
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
		std::cout << "Window could not be created! SDL_Error: " << SDL_GetError();
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
	TTF_Font* font = TTF_OpenFont("assets/octin sports free.ttf", fontSize);
	if (font == NULL) {
		std::cout << "Failed to load font! Error: " << TTF_GetError() << std::endl;
		return -1;
	}
	
	SDL_Rect rect = {100, 100, 200, 50};
	SDL_Color colour = {109, 158, 237, 255}; //Sky blue
	SDL_Color fontColour = {255, 255, 255, 255}; //Black

	UI_element element(rect, colour, font, fontColour);
	element.loadTexture(renderer, "Hello World!");

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
		}

		//Clear screen
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		//Render
		element.render(renderer);

		//Update screen from backbuffer and clear backbuffer
		SDL_RenderPresent(renderer);

		//Slow down rendering rate
		SDL_Delay(20);
	}

	/*--------------------------------Ending Programme--------------------------------*/
	//Free textures
	element.free();

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