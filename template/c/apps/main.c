#include "../include/sdl_manager.h"
#include "../include/cor.h"

SDL_Window* window = NULL; //janela principal do programa

SDL_Renderer* renderer = NULL;

SDL_Event event;

const int SCREEN_WIDTH = 1000; //largura da janela principal em pixels
const int SCREEN_HEIGHT = 800; //altura da janela principal em pixels


int main(int argc, char* argv[]){


    if(!InitializeSDL(&renderer, &window, SCREEN_WIDTH, SCREEN_HEIGHT)){
        return 1;
    }

    int quadrado_size = 200;
    SDL_Rect quadrado = {(SCREEN_WIDTH - quadrado_size) / 2, (SCREEN_HEIGHT - quadrado_size) / 2, quadrado_size, quadrado_size};

    bool quit = false;
    while(!quit){ //loop for game loop
        while(SDL_PollEvent(&event) != 0){ //loop que consome todos os eventos
            if(event.type == SDL_QUIT){
                quit = true; 
            }
        }        

        colorirFundo(verde, renderer);
        colorirRect(&quadrado, vermelho, renderer);

        SDL_RenderPresent(renderer);

    }

    SDL_Quit();

    return 0;
}