#ifndef __COORD_HPP__
#define __COORD_HPP__

#include "includes.hpp"

const int SCREEN_HEIGHT = 500;
const int SCREEN_WIDTH = 500;

const int GROUND_SPEED = 1;
const int GROUND_HEIGHT = 0;

std::string SPRITES_FOLDER = "sprites/";

enum movimentos{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

int x_ref = 0;
int y_ref = 0;

#endif