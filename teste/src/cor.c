#include "../include/cor.h"

Cor randomColor(){

    int r = rand() % 266;
    int g = rand() % 266;
    int b = rand() % 266;
    int a = 255;

    Cor ret = {r, g, b, a};

    return ret;
}

void colorirRect(SDL_Rect* rect, Cor c){
    SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
    SDL_RenderFillRect(renderer, rect);
}

void colorirFundo(Cor c){
    SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
    SDL_RenderClear(renderer); //limpa o frame atual para carregar o proximo frame
}
