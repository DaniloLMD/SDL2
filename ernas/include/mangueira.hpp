#ifndef __MANGUERA_HPP__
#define __MANGUERA_HPP__

#include "includes.hpp"

const int MANGUEIRA_WIDTH = 150;
const int MANGUEIRA_HEIGHT = 300;

class Mangueira{
    public:
        Mangueira(SDL_Renderer* renderer, int x);

        void Render();
        void Teleport(int x);
        SDL_Rect GetRect() {return rect;}
    private:
        SDL_Texture* texture;
        SDL_Renderer* renderer;
        SDL_Rect rect;
};

std::string MANGUEIRA_PATH = SPRITES_FOLDER + "mangueira.png";

#endif