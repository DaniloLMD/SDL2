#include "../include/sdl_manager.h"
#include <stdlib.h>
#include <time.h>

/**
 * @brief inicia o basico do SDL2, como janela e renderer
 * @return true, se a inicializacao foi bem sucedida, false caso contrario
*/
bool InitializeSDL(){
    srand(time(0));
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow(
        "Jogo da Velha", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
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

/**
 * @brief verifica se dois SDL_Rect estao colidindo
 * @param a primeiro rect
 * @param b segundo rect
 * @return true, se houver colisao, false caso contrario
*/
bool check_colision(SDL_Rect* a, SDL_Rect* b){
    if(!a || !b) return false;

    if(a->x + a->w <= b->x){
        return false;
    }
    if(b->x + b->w <= a->x){
        return false;
    }

    if(a->y + a->h <= b->y){
        return false;
    }
    if(b->y + b->h <= a->y){
        return false;
    }

    return true;
}

/**
 * @brief Recebe uma imagem e retorna uma textura que contem essa imagem.
 * @param image_path caminho da imagem a ser transformada em textura
 * @return texture com a imagem localizada em image_path
*/
SDL_Texture* load_image_to_texture(const char* image_path){ // Recebe o caminho para determinada imagem e cria uma textura com base nela

    SDL_Texture* texture = NULL;

    SDL_Surface* surface = IMG_Load(image_path);

    if(!surface){ // Se não conseguir achar a imagem
        printf("load_image_to_texture error: %s\n", IMG_GetError());
        return NULL;
    }

    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    return texture;
}
/**
 * @brief Coloca uma imagem dentro de um SDL_Rect
 * @param image_texture textura da imagem
 * @param rect endereco do rect a guardar a imagem
 * @return void
*/
void load_image_to_rect(SDL_Texture* image_texture, SDL_Rect* rect){
    if(image_texture == NULL) printf("%s error: invalid texture\n", __func__);
    if(rect == NULL) printf("%s error: invalid rect\n", __func__);
    
    SDL_RenderCopy(renderer, image_texture, NULL, rect);
}