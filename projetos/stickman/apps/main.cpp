#include "../include/includes.hpp"
#include "../include/ernanni.hpp"
#include "../include/ground.hpp"
#include "../include/mangueira.hpp"
#include "../include/coordenadas.hpp"
#include <iostream>

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

bool checkColision(SDL_Rect rect1, SDL_Rect rect2){
    if(rect1.x + rect1.w < rect2.x){
        return false;
    }

    if(rect2.x + rect2.w < rect1.x){
        return false;
    }

    if(rect1.y + rect1.h < rect2.y){
        return false;
    }
    return true;
}

bool InitializeSDL(){
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow(
        "Fisica 2.0", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
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

void resetGame(Ernanni* ernas, Ground* ground, Mangueira* mangada){
    quit = false;
    ernas->Teleport(50, SCREEN_HEIGHT - ERNANNI_HEIGTH);
    mangada->Teleport(SCREEN_WIDTH/2);

    ernas->UpdateMovement(PARADO);
}

int main(){

    if(!InitializeSDL()){
        return 1;
    }

    Ernanni ernas = Ernanni(renderer, 50, SCREEN_HEIGHT - ERNANNI_HEIGTH, GROUND_SPEED);
    Ground ground = Ground(renderer, 0, GROUND_HEIGHT+25, GROUND_SPEED);
    Mangueira mangada = Mangueira(renderer, SCREEN_WIDTH/2);
    SDL_Event event;

    while(!quit){
        while(SDL_PollEvent(&event) != 0){
            if(event.type == SDL_QUIT){
                quit = true;
            }
            else if(event.type == SDL_KEYDOWN){
                if(event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d){
                    if(ernas.GetMovement() == PARADO || ernas.GetMovementDirection() != RIGHT){
                        ernas.changeMovementDirection(RIGHT);
                        ernas.UpdateMovement(CORRENDO_RIGHT);
                    }
                    else ernas.UpdateMovement(PARADO);
                }
                else if(event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a){
                    if(ernas.GetMovement() == PARADO || ernas.GetMovementDirection() != LEFT){
                        ernas.changeMovementDirection(LEFT);
                        ernas.UpdateMovement(CORRENDO_LEFT);
                    }
                    else{
                        ernas.UpdateMovement(PARADO);
                    }   
                }
                else if(event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w){
                    ernas.Jump();
                }
                else if(event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s){
                    if(ernas.GetMovement() >= PULANDO && ernas.GetMovement() <= PULANDO_4){
                        ernas.caindo = true;
                    }
                }
            }
        }
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 176, 196, 222, 255);

        ground.Render();
        ernas.Update();
        ernas.Render();
        mangada.Render();

        bool changedScreen = false;
        if(ernas.GetRect().x > SCREEN_WIDTH){
            ernas.Teleport(0, ernas.GetRect().y);
            changedScreen = true;
        }
        else if(ernas.GetRect().x + ernas.GetRect().w < 0){
            ernas.Teleport(SCREEN_WIDTH, ernas.GetRect().y);
            changedScreen = true;
        }
        if(changedScreen){
            int new_x = rand()%SCREEN_WIDTH;
            if(new_x < ERNANNI_WIDTH + 25) new_x = ERNANNI_WIDTH+25;  
            mangada.Teleport(new_x);
        }

        if(checkColision(ernas.GetRect(), mangada.GetRect())){
            while(SDL_PollEvent(&event) == 0){

            }
            resetGame(&ernas, &ground, &mangada);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(3);
    }

    SDL_Quit();

    return 0;
}