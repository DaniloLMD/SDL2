#ifndef __GROUND_HPP__
#define __GROUND_HPP__

#include "includes.hpp"

class Ground{
    public:
        Ground(SDL_Renderer* renderer, int x, int y, int speed);
        void Render();

        SDL_Rect GetRect() {return rect;}
        
    private:
        SDL_Texture* texture;
        SDL_Renderer* renderer;
        SDL_Rect rect;
        int speed;
};

//sprite do chao
std::string GROUND_PATH = SPRITES_FOLDER + std::string("ground.png");

#endif