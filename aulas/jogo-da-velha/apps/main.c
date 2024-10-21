#include "../include/sdl_manager.h"
#include "../include/cor.h"
#include "../include/board.h"

int main(int argc, char* argv[]){


    if(!InitializeSDL()){
        printf("Erro na inicializacao do SDL\n");
        return 1;
    }

    bool quit = false;

    board_initialize();

    while(!quit){ //loop principal
        while(SDL_PollEvent(&event) != 0){ //loop que consome todos os eventos
            if(event.type == SDL_QUIT){
                quit = true; 
            }

            if(event.type == SDL_MOUSEBUTTONDOWN){
                handle_click();
            }
        }        

        draw_board();

        SDL_RenderPresent(renderer);
    }

    SDL_Quit();

    return 0;
}