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
        "Labirinto", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
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



////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////// Cor /////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
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
    SDL_RenderClear(renderer); //limpa o frame atual para carregar o proximo frame
}

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////// lab  ////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
const int dimensao = 40;
vector<SDL_Rect> path = {{0, 0, dimensao, dimensao}};
const int lim_x = SCREEN_WIDTH/dimensao;
const int lim_y = SCREEN_HEIGHT/dimensao;

bool valid(int x, int y){
    return (x >= 0 && x < SCREEN_WIDTH) && (y >= 0 && y < SCREEN_HEIGHT);
}

void gerar(){

    vector<pair<int,int>> movimentos = {{0,1}, {0,-1}, {1,0}, {-1,0}};  
    map<pair<int,int>, int> mp;
    SDL_Rect new_path = {0, 0, dimensao, dimensao};
    bool quit = false;
    do{
        bool moveu = false;
        int mov = rand() % 4;
        int x = new_path.x + dimensao*movimentos[mov].first;
        int y = new_path.y + dimensao*movimentos[mov].second;
        if(valid(x,y) && !mp[{x,y}]){
            moveu = true;
            mp[{x,y}]++;
            new_path.x = x;
            new_path.y = y;
            path.push_back(new_path);
        }
        if(!moveu){
            for(auto p: path){
                int ok = 4;
                for(auto pi: movimentos){
                    x = p.x + dimensao * pi.first;
                    y = p.y + dimensao * pi.second;
                    if(valid(x,y)){
                        if(mp[{x,y}]){
                            ok--;
                        }
                    }
                    else{
                        ok--;
                    }
                }
                if(ok){
                    new_path = p;
                }
            }
        }

        SDL_RenderPresent(renderer);
        colorirFundo(verde);
        for(auto x: path){
            colorirRect(&x, cinza);
        }
        colorirRect(&new_path, azul);

        while(SDL_PollEvent(&event) != 0){ //loop que consome todos os eventos
            if(event.type == SDL_QUIT){
                quit = true; 
            }
        }  
        if(quit) break;
        SDL_Delay(10);
    
    } while(!quit && !mp[{dimensao * (lim_x-1), dimensao* (lim_y-1)}]);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[]){
    srand(time(0));

    if(!InitializeSDL()){
        return 1;
    }

    bool quit = false;

    int vezes = int(1e3);
    // cout << "gerei!\n";
    while(!quit){ //loop for game loop
        while(SDL_PollEvent(&event) != 0){ //loop que consome todos os eventos
            if(event.type == SDL_QUIT){
                quit = true; 
            }
        }      

        while(vezes--){
            while(SDL_PollEvent(&event) != 0){ //loop que consome todos os eventos
                if(event.type == SDL_QUIT){
                    quit = true; 
                }
            }      

            gerar();
            path.clear();
            path.push_back({0, 0, dimensao, dimensao});

        }
        colorirFundo(verde);
        SDL_Rect test = {dimensao * (lim_x-1), dimensao * (lim_y - 1), dimensao, dimensao};
        colorirRect(&test, vermelho);

        for(auto x: path){
            colorirRect(&x, cinza);
        }

        SDL_RenderPresent(renderer);

        SDL_Delay(1);
    }

    SDL_Quit();

    return 0;
}
