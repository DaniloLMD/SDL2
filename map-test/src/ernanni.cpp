#include "../include/includes.hpp"
#include "../include/ernanni.hpp"
#include "../include/coordenadas.hpp"

Ernanni::Ernanni(SDL_Renderer* renderer, int x, int y, int speed) : renderer(renderer), speed(speed)
{
    
    SDL_Surface* surface = IMG_Load(ERNANNI_PATH.c_str());
    texture = SDL_CreateTextureFromSurface(renderer, surface);

    rect = {x, y, ERNANNI_WIDTH, ERNANNI_HEIGTH};
    SDL_FreeSurface(surface);
}

void Ernanni::Update(int movement, int possible){
    switch(movement){
        case UP:
            if(!possible) rect.y -= speed;
            if(rect.y < 0) rect.y = 0;
            break;

        case DOWN:
            if(!possible) rect.y += speed;
            if(rect.y + rect.h > SCREEN_HEIGHT) rect.y = SCREEN_HEIGHT - rect.h;
            break;

        case LEFT:
            rect.x -= speed;
            if(rect.x < 0) rect.x = 0;
            break;

        case RIGHT:
            rect.x += speed;
            if( rect.x + rect.w >= SCREEN_WIDTH ) rect.x = SCREEN_WIDTH - rect.w;
            break;
    }
}

void Ernanni::Render(){
    SDL_RenderCopy(renderer, texture, NULL, &rect);
}
