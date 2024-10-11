#ifndef __SDL_MANAGER_HPP__
#define __SDL_MANAGER_HPP__

#include <SDL2/SDL.h>   

bool InitializeSDL(SDL_Renderer** renderer, SDL_Window** window, int SCREEN_WIDTH, int SCREEN_HEIGHT);

bool check_colision(SDL_Rect* a, SDL_Rect* b);

#endif