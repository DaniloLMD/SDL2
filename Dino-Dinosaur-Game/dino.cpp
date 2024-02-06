#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

using namespace std;



//----------------------------------------------------//
//----------------  VARIAVEIS GLOBAIS ----------------//
//----------------------------------------------------//



//----------------  Variaveis Normais ----------------
const char* SPRITES_FOLDER = "sprites/";

bool quit = false; //booleano que controla o evento do botao de fechar o programa


//----------------  Variaveis do SDL -----------------
SDL_Window* window = NULL; //janela principal do programa
const int SCREEN_WIDTH = 800; //largura da janela principal em pixels
const int SCREEN_HEIGHT = 400; //altura da janela principal em pixels

SDL_Renderer* renderer = NULL;

SDL_Event event;

SDL_Texture *dinoTexture = NULL;


//----------------  Variaveis do Dino ----------------
int dinoX = 100;
int dinoY = SCREEN_HEIGHT - 48 - 40;



//----------------------------------------------------//
//----------------  FUNCOES DO SDL -------------------//
//----------------------------------------------------//

/**
 * @brief inicializa o SDL, criando uma janela e um renderer
 * @return true, se conseguiu criar tanto a janela quanto o renderer ou false caso contrario
*/
bool InitializeSDL(){
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow(
        "Dino Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN
    );

    if(!window){
        cout << "SDL_CreateWindow Error: " << SDL_GetError << endl;
        SDL_Quit();
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if(!renderer){
        cout << "SDL_CreateRenderer Error: " << SDL_GetError << endl;
        SDL_Quit();
        return false;
    }

    //SDL_Delay(5000);

    return true;
}   

/**
 * @brief carrega a imagem do dinossauro
*/
bool LoadMedia(){
    string dinoImagePath = SPRITES_FOLDER + string("dino_0.png");

    SDL_Surface* dinoSurface = IMG_Load(dinoImagePath.c_str());

    if(!dinoSurface){
        cout << "Problem loading dino_0.png "<< IMG_GetError() << endl;
        return false;
    }

    dinoTexture = SDL_CreateTextureFromSurface(renderer, dinoSurface);

    SDL_FreeSurface(dinoSurface);

    return true;
} 


int main(int argc, char* argv[]){
    
    if(!InitializeSDL()){
        return 1;
    }

    if(!LoadMedia()){
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();

        return 1;
    }

    while(!quit){ //loop for game loop
        while(SDL_PollEvent(&event) != 0){ //loop que aguarda algum evento
            if(event.type == SDL_QUIT){
                quit = true; 
            }
        }

        SDL_RenderClear(renderer); //limpa o frame atual para carregar o proximo frame

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        SDL_Rect dinoRect = {dinoX, dinoY, 44, 48};

        SDL_RenderCopy(renderer, dinoTexture, NULL, &dinoRect);

        SDL_RenderPresent(renderer);
    }

    SDL_Quit();

    return 0;
}