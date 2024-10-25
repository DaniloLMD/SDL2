#include <stdio.h>
#include <SDL2/SDL.h>

typedef char bool;
#define true 1
#define false 0

SDL_Window* window = NULL; //janela principal do programa

SDL_Renderer* renderer = NULL;

SDL_Event event;

const int SCREEN_WIDTH = 1000; //largura da janela principal em pixels
const int SCREEN_HEIGHT = 800; //altura da janela principal em pixels


/**
 * @brief inicia o basico do SDL2, como janela e renderer
 * @return true, se a inicializacao foi bem sucedida, false caso contrario
*/
bool InitializeSDL(){
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow(
        "SDL2 C Project", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN
    );

    if(!window){
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(!renderer){
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_Quit();
        return false;
    }

    return true;
}

void colorirFundo(int r, int g, int b, int a){
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    SDL_RenderClear(renderer); 
}

void colorirRect(SDL_Rect* rect, int r, int g, int b, int a){
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    SDL_RenderFillRect(renderer, rect);
}


int main(int argc, char* argv[]){


    if(!InitializeSDL()){
        printf("Erro na inicializacao do SDL\n");
        return 1;
    }

    int quadrado_size = 200;
                                //{x, y, largura (w), altura(h)}
    SDL_Rect quadrado_colorido = {0, 0, quadrado_size, quadrado_size};


    bool quit = false;
    while(!quit){ //loop principal
        while(SDL_PollEvent(&event) != 0){ //loop que consome todos os eventos
            if(event.type == SDL_QUIT){
                quit = true; 
            }

            //se houve click com o mouse, o quadrado é movido para onde foi clicado
            if(event.type == SDL_MOUSEBUTTONDOWN){
                int mouseX, mouseY; //coordenadas da posicao do mouse
                SDL_GetMouseState(&mouseX, &mouseY);
                printf("clicou em (%d, %d)\n", mouseX, mouseY);
                quadrado_colorido.x = mouseX - quadrado_size/2;
                quadrado_colorido.y = mouseY - quadrado_size/2;
            }

            //se houve interação com o teclado, o quadrado move na direção das setinhas
            if(event.type == SDL_KEYDOWN){
                int movement_distance = 20;
                int sum_x = 0, sum_y = 0;
                if(event.key.keysym.sym == SDLK_RIGHT) sum_x = movement_distance;
                if(event.key.keysym.sym == SDLK_LEFT) sum_x = -movement_distance;
                if(event.key.keysym.sym == SDLK_UP) sum_y = -movement_distance;
                if(event.key.keysym.sym == SDLK_DOWN) sum_y = movement_distance;


                //checando se o movimento é valido (nao ultrapassou o limite da tela)
                bool valid_movement = true;
                //ultrapassou a borda direita
                if(quadrado_colorido.x + quadrado_colorido.w + sum_x > SCREEN_WIDTH) valid_movement = false;
                //ultrapassou a borda esquerda
                if(quadrado_colorido.x + sum_x < 0) valid_movement = false;
                //ultrapassou a borda de cima
                if(quadrado_colorido.y + sum_y < 0) valid_movement = false;
                //ultrapassou a borda de baixo
                if(quadrado_colorido.y + quadrado_colorido.h + sum_y > SCREEN_HEIGHT) valid_movement = false;

                if(valid_movement){
                    quadrado_colorido.x += sum_x;
                    quadrado_colorido.y += sum_y;
                }
                
            }

        }        

        colorirFundo(115, 168, 50, 255); //colorindo fundo com uma tonalidade de verde
        colorirRect(&quadrado_colorido, 255, 0, 0, 255); //colorindo o rect de vermelho

        SDL_RenderPresent(renderer);
    }

    SDL_Quit();

    return 0;
}