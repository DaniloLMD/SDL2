#ifndef __COR_H__
#define __COR_H__

#include "sdl_manager.h"

typedef struct cor{
    int r,g,b,a;

} Cor;

Cor vermelho = {255, 0, 0, 255};
Cor verde = {115, 168, 50, 255};
Cor azul = {0, 0, 255, 0};
Cor marrom = {168, 123, 50, 255};
Cor cinza = {203, 212, 201, 255};
Cor branco = {255, 255, 255};
Cor preto = {0, 0, 0};


Cor randomColor();

void colorirRect(SDL_Rect* rect, Cor c);

void colorirFundo(Cor c);

#endif