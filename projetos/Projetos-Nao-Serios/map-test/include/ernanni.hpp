#ifndef __ERNANNI_HPP__
#define __ERNANNI_HPP__

#include "includes.hpp"
#include "coordenadas.hpp"

const int ERNANNI_WIDTH = 50;
const int ERNANNI_HEIGTH = 50;

class Ernanni{
    public:
        Ernanni(SDL_Renderer* renderer, int x, int y, int speed);

        void Update(int movement, int possible);
        void Render();

        SDL_Rect GetRect() {return rect;}

    private:
        SDL_Texture* texture;
        SDL_Renderer* renderer;
        SDL_Rect rect;
        int speed;
};

//sprites do ernanni
std::string ERNANNI_PATH = SPRITES_FOLDER + "ernanni.jpg";

#endif