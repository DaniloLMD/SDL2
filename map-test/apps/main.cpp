#include "../include/includes.hpp"
#include "../include/ernanni.hpp"
#include "../include/ground.hpp"
#include "../include/coordenadas.hpp"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

bool InitializeSDL(){
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow(
        "Map Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN
    );

    if(!window){
        std::cout << "SDL_CreateWindow Error " << SDL_GetError << std::endl;
        SDL_Quit();
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(!renderer){
        std::cout << "SDL_CreateRenderer Error " << SDL_GetError << std::endl;
        SDL_Quit();
        return false;
    }

    return true;
}

bool quit = false;

int main(){

    if(!InitializeSDL()){
        return 1;
    }

    int ernas_speed = 10;
    int ground_speed = 10;
    Ernanni ernas = Ernanni(renderer, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, ernas_speed);
    Ground ground = Ground(renderer, 0, 0, ground_speed);
    SDL_Event event;

    while(!quit){
        while(SDL_PollEvent(&event) != 0){
            if(event.type == SDL_QUIT){
                quit = true;
            }
            else if(event.type == SDL_KEYDOWN){
                int type_of_movement = -1;
                if(event.key.keysym.sym == SDLK_RIGHT){
                    type_of_movement = RIGHT;
                }
                else if(event.key.keysym.sym == SDLK_LEFT){
                    type_of_movement = LEFT;
                }
                else if(event.key.keysym.sym == SDLK_UP){
                    type_of_movement = UP;
                }
                else if(event.key.keysym.sym == SDLK_DOWN){
                    type_of_movement = DOWN;
                }

                if(type_of_movement != -1)
                    ernas.Update(type_of_movement,ground.Update(type_of_movement));
            }
        }
        SDL_RenderClear(renderer);
        //SDL_SetRenderDrawColor(renderer, 176, 196, 222, 255);

        ground.Render();
        ernas.Render();

        SDL_RenderPresent(renderer);
        //SDL_Delay(2);
    }


    SDL_Quit();

    return 0;
}