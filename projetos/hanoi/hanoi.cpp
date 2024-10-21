#include <SDL2/SDL.h>
#include <iostream>
#include <vector>

#include <unistd.h>

using namespace std;

SDL_Window* window = NULL; //janela principal do programa

SDL_Renderer* renderer = NULL;

SDL_Event event;

const int SCREEN_WIDTH = 1000; //largura da janela principal em pixels
const int SCREEN_HEIGHT = 800; //altura da janela principal em pixels

struct cor{
    int r,g,b,a;

    bool operator==(cor x){
        return r == x.r && g == x.g && b == x.b && a == x.a;
    }
};

cor marrom = {168, 123, 50, 255};
cor cinza = {203, 212, 201, 255};
cor verde = {115, 168, 50, 255};

cor randomColor(){

    int r = rand() % 266;
    int g = rand() % 266;
    int b = rand() % 266;
    int a = 255;

    return {r, g, b, a};
}

void colorirRect(SDL_Rect* rect, cor c){
    SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
    SDL_RenderFillRect(renderer, rect);
}

void colorirFundo(cor c){
    SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
    SDL_RenderClear(renderer); //limpa o frame atual para carregar o proximo frame
}


/**
 * @brief inicializa o SDL, criando uma janela e um renderer
 * @return true, se conseguiu criar tanto a janela quanto o renderer ou false caso contrario
*/
bool InitializeSDL(){
    SDL_Init(SDL_INIT_VIDEO);

    //inicializando a janela
    window = SDL_CreateWindow(
        "Hanoi", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN
    );

    if(!window){
        SDL_Quit();
        return false;
    } 

    //inicializando o renderer (para carregar coisas na janela)
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(!renderer){
        SDL_Quit();
        return false;
    }

    return true;
}   

int main(int argc, char* argv[]){
    srand(time(0));

    //gerando a base
    int altura_base = 100, largura_base = SCREEN_WIDTH - 50;
    SDL_Rect base = {SCREEN_WIDTH / 2 - largura_base / 2, SCREEN_HEIGHT - 50 - altura_base / 2, largura_base, altura_base};
    
    //gerando os paus    
    int largura_pau = 75, altura_pau = 500;
    int distancia_paus = 325;
    vector<SDL_Rect> paus;
    paus.push_back({base.x + 150, base.y - altura_pau, largura_pau, altura_pau});
    paus.push_back({paus[0].x + distancia_paus, base.y - altura_pau , largura_pau, altura_pau});
    paus.push_back({paus[1].x + distancia_paus, base.y - altura_pau , largura_pau, altura_pau});

    //gerando os discos
    int qtd_discos = 5;
    vector<SDL_Rect> discos(qtd_discos);
    vector<int> largura_discos(qtd_discos);
    int altura_discos = 100;
    for(int i = 0; i < qtd_discos; i++){
        largura_discos[i] = largura_pau + 25 + 50 * (qtd_discos - 1 -i);
        discos[i] = {paus[0].x - (largura_discos[i] - largura_pau) / 2, paus[0].y + paus[0].h - altura_discos*(i+1), largura_discos[i], altura_discos};
    }



    if(!InitializeSDL()){
        return 1;
    }

    bool quit = false;
    //gerando cores
    vector<cor> cores(qtd_discos);
    
    for(int i = 0; i < qtd_discos; i++){
        cor c;
        while(1){
            c = randomColor();
            int unico = 1;
            for(int j = 0; j < i; j++){
                if(c == cores[j]){
                    unico = 0;
                    break;
                }
            }
            if(unico) break;
        }
        cores[i] = c;
    }

    while(!quit){ //loop for game loop

        while(SDL_PollEvent(&event) != 0){ //loop que consome todos os eventos
            if(event.type == SDL_QUIT){
                quit = true; 
            }
        }        
        colorirFundo(verde);

        colorirRect(&base, marrom);
        for(auto x: paus){
            colorirRect(&x, cinza);
        }

        for(int i = 0; i < qtd_discos; i++){
            colorirRect(&discos[i], cores[i]);
        }


        SDL_RenderPresent(renderer);

        SDL_Delay(5);
    }

    SDL_Quit();

    return 0;
}
