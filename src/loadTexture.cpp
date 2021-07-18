#include "loadTexture.h"

//A wrapper function for creating texture from surface
SDL_Texture* createTextureFromSurface(SDL_Renderer* renderer, SDL_Surface* surface) {
	SDL_Texture* texture = nullptr;
	 if (surface == nullptr) {
        std::cout << "Unable to render text surface! Error: " << TTF_GetError() << std::endl; 
    } else {
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (texture == nullptr) {
            std::cout << "Unable to create texture form rendered text! Error: " << SDL_GetError() << std::endl;
        } 
        SDL_FreeSurface(surface);
    }
	return texture;
}

//load font
TTF_Font* loadFont(const int fontSize) {
	TTF_Font* font = nullptr;
	font = TTF_OpenFont("assets/octin sports free.ttf", fontSize);
	if (font == nullptr) {
		std::cout << "Failed to load cell font! Error: " << TTF_GetError() << std::endl;
	}
	return font;
};

//Load texture for text
SDL_Texture* loadTexture(SDL_Renderer* renderer, TTF_Font* font, const SDL_Color& colour, const char* text) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, colour);
	//Return loaded texture if successfull otherwise a nullptr
	return createTextureFromSurface(renderer, surface);
}

//Load texture from bmp (function overriding)
SDL_Texture* loadTexture(SDL_Renderer* renderer, const char* relativePath) {
	SDL_Surface* surface = SDL_LoadBMP(relativePath);
	// make white pixel transparent
	SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 255, 255, 255));
	//Return loaded texture if successfull otherwise a nullptr
	return createTextureFromSurface(renderer, surface);
}
