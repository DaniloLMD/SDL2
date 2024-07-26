#ifndef __LUFFY_HPP__
#define __LUFFY_HPP__

using namespace std;
#include <string>
#include <map>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Animation{
    public:
        Animation() : x(0), y(0), w(0), h(0), f(0),s(0) {}
        
        Animation(int x, int y, int w, int h, int f, int s) : x(x), y(y), w(w), h(h), f(f), s(s)
        {

        }

    private:
        int x,y,w,h,f,s;
        int current_frame;
};

class Luffy{
    public:
        map<string, Animation> animations;

        Luffy(){
            animations["idle"] = Animation(0, 0, 40, 50, 7, 100);
            animations["walking"] = Animation(0, 1, 40, 50, 6, 100);
        }

        void walk(){

        }

    private:    
        string sprite_file_path = "sprites/luffy_sprites.png";
};

#endif