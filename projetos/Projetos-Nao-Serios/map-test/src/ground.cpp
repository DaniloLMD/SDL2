#include "../include/ground.hpp"
#include "../include/coordenadas.hpp"

Ground::Ground(SDL_Renderer* renderer, int x, int y, int speed) : renderer(renderer) , speed(speed)
{
    SDL_Surface* surface = IMG_Load(GROUND_PATH.c_str());
    texture = SDL_CreateTextureFromSurface(renderer, surface);

    w = surface->w;
    h = surface->h;

    rect = {x, y, w, h};
    SDL_FreeSurface(surface);
}

void Ground::Render(){
    SDL_RenderCopy(renderer, texture, NULL, &rect);
}

int Ground::Update(int movement){

    switch(movement){
        case UP:
            if(-y_ref <= 0) return 0;
            rect.y += speed;
            y_ref += speed;

            break;

        case DOWN:
            if( -y_ref + SCREEN_HEIGHT > h ) return 0;
            rect.y -= speed;
            y_ref -= speed;

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
    return 1;
}


// -850 ate 