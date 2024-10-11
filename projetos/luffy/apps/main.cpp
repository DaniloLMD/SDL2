#include <SDL2/SDL.h>
#include <bits/stdc++.h>
#include <unistd.h>

using namespace std;

#define dbg(x) cout << #x << " = " << x << '\n';


////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////// SDL /////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

SDL_Window* window = NULL; //janela principal do programa

SDL_Renderer* renderer = NULL;

SDL_Event event;

const int SCREEN_WIDTH = 1000; //largura da janela principal em pixels
const int SCREEN_HEIGHT = 800; //altura da janela principal em pixels

bool InitializeSDL(){
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow(
        "Macaco do Luffy", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
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



////////////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[]){
    srand(time(0));

    if(!InitializeSDL()){
        return 1;
    }

    bool quit = false;
    
    while(!quit){ //loop for game loop
        while(SDL_PollEvent(&event) != 0){ //loop que consome todos os eventos
            if(event.type == SDL_QUIT){
                quit = true; 
            }
            else if(event.type == SDL_KEYDOWN){
            }   

        }        

        SDL_RenderPresent(renderer);

        SDL_Delay(70);
    }

    SDL_Quit();

    return 0;
}
