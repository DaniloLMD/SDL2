#include "geral.h"

bool InitializeSDL(){  // Inicia o SDL, fazendo coisas como criar janela e alocar o renderer

    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow(  // Inicializando a janela
        "Haja Paciência", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN
    );

    if(!window){ // Se não conseguir inicializar a janela 
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit(); 
        return false;
    }

    // Inicializando o renderizador (para renderizar coisas na janela)
    renderer = SDL_CreateRenderer(window, -1, 0);

    if(!renderer){ // Se não conseguir inicializar o rendererizador
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_Quit();
        return false;
    }

    return true;
}

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

bool is_clicking_on_rect(SDL_Rect* r){ // Verifica se o usuário clicou em determinado rect
    if(!clicked) return false;
    if(mouseX < r->x) return false;
    if(mouseX > r->x + r->w) return false;
    if(mouseY < r->y) return false;
    if(mouseY > r->y + r->h) return false;
    return true;
}
