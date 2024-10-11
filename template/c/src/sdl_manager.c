#include "../include/sdl_manager.h"
#include <stdlib.h>
#include <time.h>

bool InitializeSDL(SDL_Renderer** renderer_ref, SDL_Window** window_ref, int SCREEN_WIDTH, int SCREEN_HEIGHT){
    
    SDL_Renderer *renderer = *renderer_ref;
    SDL_Window *window = *window_ref;

    srand(time(0));
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow(
        "SDL2 C Project", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN
    );

    if(!window){
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(!renderer){
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_Quit();
        return false;
    }

    *window_ref = window;
    *renderer_ref = renderer;

    return true;
}

bool check_colision(SDL_Rect* a, SDL_Rect* b){
    if(!a || !b) return false;

    if(a->x + a->w <= b->h){
        return false;
    }
    if(b->x + b->w <= a->x){
        return false;
    }

    if(a->y + a->h <= b->y){
        return false;
    }
    if(b->y + b->h <= a->y){
        return false;
    }

    return true;
}