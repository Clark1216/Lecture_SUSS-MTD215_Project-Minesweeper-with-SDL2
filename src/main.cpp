/*A Minesweeper made made using SDL and SDL_ttf*/
#include <iostream>
#include "minesweeper.h"

//SDL requires c style paramaters in the main function
int main( int argc, char* args[] ) {
	
	Minesweeper minesweeper;
	minesweeper.play();

	//Exit programme
	std::cout << "Exiting programme..." << std::endl;
	return 0;
}