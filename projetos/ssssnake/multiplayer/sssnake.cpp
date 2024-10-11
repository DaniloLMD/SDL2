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
        "Cobra Cobra Cobra", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
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
////////////////////////////////////////// SSSNAKE /////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

enum movements{
    STOP = -1, UP, DOWN, LEFT, RIGHT, WIN
};

cor cor_cobra = {0, 100, 0 , 255};
cor cor_cabeca = {50, 205, 50, 255};
cor cor_cabeca2 = { 0, 0, 255, 255};
cor cor_cobra2 = {30, 144, 255, 255};
int dimensao_cobra = 40;
vector<SDL_Rect> snake(1);
vector<SDL_Rect> snake2(1);
SDL_Rect apple = {0,0,dimensao_cobra, dimensao_cobra};

int movimento = LEFT;
int movimento2 = RIGHT;
int last_mov = STOP;
int last_mov2 = STOP;

vector<pair<int,int>> alteracoes = { {0, -1}, {0, 1}, {-1,0}, {1,0}, {0,0}};

bool check_colision(SDL_Rect a, SDL_Rect b){
    if(a.x + a.w <= b.x){
        return false;
    }
    if(b.x + b.w <= a.x){
        return false;
    }

    if(a.y + a.h <= b.y){
        return false;
    }
    if(b.y + b.h <= a.y){
        return false;
    }

    return true;
}

void apple_spawn();
void update_snake(){
    last_mov = movimento;
    //checando se comeu a maçã
    if(check_colision(snake[0], apple)){
        apple_spawn();
        SDL_Rect new_snake = {snake.back().x, snake.back().y,dimensao_cobra,dimensao_cobra};
        snake.push_back(new_snake);
    }

    cor corpo = cor_cobra;
    cor cabeca = cor_cabeca;
    if(movimento != STOP ){
        int x, y;
        x = snake[0].x + dimensao_cobra * alteracoes[movimento].first; 
        y = snake[0].y + dimensao_cobra * alteracoes[movimento].second;

        if(x < 0){
            x = SCREEN_WIDTH - snake[0].w;  
        } 
        if(x+snake[0].w > SCREEN_WIDTH){
            x = 0;
        }

        if(y < 0){
            y = SCREEN_HEIGHT - snake[0].h;
        }
        if(y+snake[0].h > SCREEN_HEIGHT){
            y = 0;
        }

        if(x >= 0 && (x+snake[0].w) <= SCREEN_WIDTH && y >= 0 && (y+snake[0].h) <= SCREEN_HEIGHT && movimento != WIN){
            for(int i = snake.size() - 1; i > 0; i--){
                snake[i] = snake[i-1];
            }

            snake[0].x = x;
            snake[0].y = y;   
        }
    }
    else{
        cabeca = {255, 69, 0, 255};
        corpo = {139, 0, 0, 255};
    }

    for(int i = 0; i < snake.size(); i++){
        for(int j = i+1; j < snake.size(); j++){
            if(check_colision(snake[i], snake[j]) && movimento != WIN){
                movimento = STOP;
            }
        }
    }
    
    for(auto x: snake2){
        if(check_colision(x, snake[0])){
            movimento = STOP;
        }
    }
    


    for(auto x: snake){
        colorirRect(&x, corpo);
    }
    colorirRect(&snake[0], cabeca);
    colorirRect(&apple, vermelho);
}

void update_snake2(){
    last_mov2 = movimento2;
    //checando se comeu a maçã
    if(check_colision(snake2[0], apple)){
        apple_spawn();
        SDL_Rect new_snake = {snake2.back().x, snake2.back().y,dimensao_cobra,dimensao_cobra};
        snake2.push_back(new_snake);
    }

    cor corpo = cor_cobra2;
    cor cabeca = cor_cabeca2;
    if(movimento2 != STOP ){
        int x, y;
        x = snake2[0].x + dimensao_cobra * alteracoes[movimento2].first; 
        y = snake2[0].y + dimensao_cobra * alteracoes[movimento2].second;

        if(x < 0){
            x = SCREEN_WIDTH - snake2[0].w;  
        } 
        if(x+snake2[0].w > SCREEN_WIDTH){
            x = 0;
        }

        if(y < 0){
            y = SCREEN_HEIGHT - snake2[0].h;
        }
        if(y+snake2[0].h > SCREEN_HEIGHT){
            y = 0;
        }

        if(x >= 0 && (x+snake2[0].w) <= SCREEN_WIDTH && y >= 0 && (y+snake2[0].h) <= SCREEN_HEIGHT && movimento2 != WIN){
            for(int i = snake2.size() - 1; i > 0; i--){
                snake2[i] = snake2[i-1];
            }

            snake2[0].x = x;
            snake2[0].y = y;   
        }
    }
    else{
        cabeca = {255, 69, 69, 255};
        corpo = {200, 30, 30, 255};
    }

    for(int i = 0; i < snake2.size(); i++){
        for(int j = i+1; j < snake2.size(); j++){
            if(check_colision(snake2[i], snake2[j]) && movimento2 != WIN){
                movimento2 = STOP;
            }
        }
    }

    for(auto x: snake){
        if(check_colision(x, snake2[0])){
            movimento2 = STOP;
        }
    }


    for(auto x: snake2){
        colorirRect(&x, corpo);
    }
    colorirRect(&snake2[0], cabeca);
    colorirRect(&apple, vermelho);
}

int limx = SCREEN_WIDTH / dimensao_cobra;
int limy = SCREEN_HEIGHT / dimensao_cobra;
void reset(){
    snake.resize(1);
    snake2.resize(1);
    movimento = UP;
    movimento2 = DOWN;
    snake[0] = {dimensao_cobra * limx/2 - dimensao_cobra, 0, dimensao_cobra, dimensao_cobra};
    snake2[0] = {dimensao_cobra * limx/2 + dimensao_cobra, 0, dimensao_cobra, dimensao_cobra};
    apple_spawn();
}

void apple_spawn(){
    bool ok = true;
    do{ 
        ok = true;
        int x = rand() % (limx);
        int y = rand() % (limy);
        x *= dimensao_cobra;
        y *= dimensao_cobra;
        apple.x = x;
        apple.y = y;
        for(auto x: snake){
            if(check_colision(x, apple)){
                ok = false;
                break;
            }
        }
        for(auto x: snake2){
            if(check_colision(x, apple)){
                ok = false;
                break;
            }
        }
    } while(!ok);

    colorirRect(&apple, vermelho);
}



////////////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[]){
    srand(time(0));

    cout << "Cobra verde: setinhas\nCobra azul: WASD\n";

    if(!InitializeSDL()){
        return 1;
    }

    bool quit = false;

    reset();
    
    while(!quit){ //loop for game loop
        while(SDL_PollEvent(&event) != 0){ //loop que consome todos os eventos
            if(event.type == SDL_QUIT){
                quit = true; 
            }
            
            else if(event.type == SDL_KEYDOWN){

                if(event.key.keysym.sym == SDLK_SPACE && (movimento == STOP || movimento2 == STOP)){
                    reset();
                }
                
                if(movimento != STOP && movimento2 != STOP){
                    //cobra 1
                    if(event.key.keysym.sym == SDLK_RIGHT && movimento != LEFT && last_mov != LEFT){
                        movimento = RIGHT;
                    }
                    else if(event.key.keysym.sym == SDLK_LEFT && movimento != RIGHT && last_mov != RIGHT){
                        movimento = LEFT;
                    }
                    else if(event.key.keysym.sym == SDLK_UP && movimento != DOWN && last_mov != DOWN){
                        movimento = UP;
                    }
                    else if(event.key.keysym.sym == SDLK_DOWN && movimento != UP && last_mov != UP){
                        movimento = DOWN;
                    }

                    //cobra 2
                    else if(event.key.keysym.sym == SDLK_d && movimento2 != LEFT && last_mov2 != LEFT){
                        movimento2 = RIGHT;
                    }
                    else if(event.key.keysym.sym == SDLK_a && movimento2 != RIGHT && last_mov2 != RIGHT){
                        movimento2 = LEFT;
                    }
                    else if(event.key.keysym.sym == SDLK_w && movimento2 != DOWN && last_mov2 != DOWN){
                        movimento2 = UP;
                    }
                    else if(event.key.keysym.sym == SDLK_s && movimento2 != UP && last_mov2 != UP){
                        movimento2 = DOWN;
                    }
                }
            }   

        }        
        if(movimento == STOP || movimento2 == STOP){
            if(movimento == STOP && movimento2 != STOP){
                movimento2 = WIN;
            }
            else if(movimento != STOP && movimento2 == STOP){
                movimento = WIN;
            }
        }

        colorirFundo(verde);
        update_snake();
        update_snake2();

        SDL_RenderPresent(renderer);

        SDL_Delay(70);
    }

    SDL_Quit();

    return 0;
}
