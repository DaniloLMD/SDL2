#include "../include/ground.hpp"
#include "../include/coordenadas.hpp"

Ground::Ground(SDL_Renderer* renderer, int x, int y, int speed) : renderer(renderer), speed(speed)
{
    SDL_Surface* surface = IMG_Load(GROUND_PATH.c_str());
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    rect = {x, y, surface->w, 120};
    SDL_FreeSurface(surface);

}

void Ground::Render(){
    SDL_RenderCopy(renderer, texture, NULL, &rect);
}
