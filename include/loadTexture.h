#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>

//A wrapper function for creating texture from surface
SDL_Texture* createTextureFromSurface(SDL_Renderer* renderer, SDL_Surface* surface);

//Load texture for text
SDL_Texture* loadTexture(SDL_Renderer* renderer, TTF_Font* font, const SDL_Color& colour, const char* text);

//Load texture from bmp (function overriding)
SDL_Texture* loadTexture(SDL_Renderer* renderer, const char* relativePath);