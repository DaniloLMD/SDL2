#include "../include/mangueira.hpp"

Mangueira::Mangueira(SDL_Renderer* renderer, int x) : renderer(renderer)
{
    SDL_Surface* surface = IMG_Load(MANGUEIRA_PATH.c_str());
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    rect = {x, SCREEN_HEIGHT - MANGUEIRA_HEIGHT, MANGUEIRA_WIDTH, MANGUEIRA_HEIGHT};
} 

void Mangueira::Render(){
    SDL_RenderCopy(renderer, texture, NULL, &rect);
}

void Mangueira::Teleport(int x){
    rect.x = x;
}
