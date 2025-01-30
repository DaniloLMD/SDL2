#ifndef __SDL_MANAGER_H__
#define __SDL_MANAGER_H__

#include <algorithm>
#include <iostream>
#include <vector>

#include <SDL2/SDL.h>   
#include <SDL2/SDL_image.h>

#define bool char
#define true 1
#define false 0

SDL_Window* window = NULL; //janela principal do programa

SDL_Renderer* renderer = NULL;

SDL_Event event;

const int SCREEN_WIDTH = 1000; //largura da janela principal em pixels
const int SCREEN_HEIGHT = 800; //altura da janela principal em pixels


bool InitializeSDL();

bool check_colision(SDL_Rect* a, SDL_Rect* b);

SDL_Texture* load_image_to_texture(const char* image_path);

void load_image_to_rect(SDL_Texture* image_texture, SDL_Rect* rect);

#endif