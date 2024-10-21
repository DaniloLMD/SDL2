#ifndef __BOARD_H__
#define __BOARD_H__

#include "sdl_manager.h"

SDL_Rect board[3][3];
SDL_Texture* board_textures[3][3];
SDL_Rect board_images[3][3];

const int board_images_rect_size = 100;

SDL_Texture* x_texture; 
SDL_Texture* o_texture; 

void board_initialize();
void draw_board_lines();
void draw_board();

void handle_click();

#endif