#ifndef __COR_HPP__
#define __COR_HPP__

#include <SDL2/SDL.h>

typedef struct cor{
    int r,g,b,a;

} Cor;

Cor marrom = {168, 123, 50, 255};
Cor cinza = {203, 212, 201, 255};
Cor verde = {115, 168, 50, 255};
Cor vermelho = {255, 0, 0, 255};

Cor randomColor();

void colorirRect(SDL_Rect* rect, Cor c, SDL_Renderer* renderer);

void colorirFundo(Cor c, SDL_Renderer* renderer);

#endif