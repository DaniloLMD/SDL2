#ifndef __SDL_MANAGER_H__
#define __SDL_MANAGER_H__

#include <SDL2/SDL.h>   

#define bool char
#define true 1
#define false 0

bool InitializeSDL(SDL_Renderer** renderer, SDL_Window** window, int SCREEN_WIDTH, int SCREEN_HEIGHT);

bool check_colision(SDL_Rect* a, SDL_Rect* b);

#endif