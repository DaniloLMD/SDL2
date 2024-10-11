#include <bits/stdc++.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
using namespace std;

#define dbg(x) cout << #x << " = " << x << '\n';

SDL_Window* window;
SDL_Renderer* renderer = NULL;
SDL_Event event;
const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 700;

//mouse
bool clickou = false;
int clicks;
int mouseX = 0, mouseY = 0;

bool quit = false;

struct cor{
    int r,g,b,a;

    bool operator==(cor x){
        return r == x.r && g == x.g && b == x.b && a == x.a;
    }
};

cor marrom = {168, 123, 50, 255};
cor cinza = {203, 212, 201, 255};
cor verde = {115, 168, 50, 255};
cor vermelho = {255, 0, 0, 255};
cor azul = {0, 0, 255, 255};

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
    // SDL_RenderPresent(renderer);
    SDL_RenderClear(renderer); //limpa o frame atual para carregar o proximo frame
}

bool InitializeSDL(){
    SDL_Init(SDL_INIT_VIDEO);

    //inicializando a janela
    window = SDL_CreateWindow(
        "Retas.", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN
    );

    if(!window){
        cout << "SDL_CreateWindow Error: " << SDL_GetError << endl;
        SDL_Quit();
        return false;
    }

    //inicializando o renderer (para carregar coisas na janela)
    renderer = SDL_CreateRenderer(window, -1, 0);//SDL_RENDERER_ACCELERATED);
    if(!renderer){
        cout << "SDL_CreateRenderer Error: " << SDL_GetError << endl;
        SDL_Quit();
        return false;
    }

    return true;
}

int main(){
    srand(time(NULL));

    InitializeSDL();


    SDL_Point p1, p2;
    vector<pair<SDL_Point,pair<SDL_Point, cor>>> retas;


    while(!quit){
        SDL_GetMouseState(&mouseX, &mouseY);

        bool montar = false;

        while(SDL_PollEvent(&event) != 0){ //loop que consome todos os eventos
            if(event.type == SDL_QUIT){
                quit = true;
            }
            if(event.type == SDL_MOUSEBUTTONDOWN){
                if(!clickou) p1 = {mouseX, mouseY};
                else{
                    p2 = {mouseX, mouseY};
                    montar = true;
                }
                clickou = !clickou;
            }
        }
        colorirFundo(verde);

        // SDL_RenderClear(renderer);
        if(montar){
            montar = false;
            retas.push_back({p1, {p2, randomColor()}});
        }

        for(auto p: retas){
            // SDL_RenderClear(renderer);
            SDL_SetRenderDrawColor(renderer, p.second.second.r, p.second.second.g, p.second.second.b, 255);
            SDL_RenderDrawLine(renderer, p.first.x, p.first.y, p.second.first.x, p.second.first.y);
        }

        SDL_RenderPresent(renderer);

        SDL_Delay(10);
    }

    SDL_Quit();

    return 0;
}
