#include "../include/sdl_manager.h"
#include "../include/cor.h"

#include <math.h>

#define PI acos(-1)

void draw_circle(int x, int y, int raio, Cor cor){
    int points = 600;
    double ang = 0;
    double var = 2 * PI / points;

    int tam = 5;
    for(int i = 0; i < points; i++, ang += var){
        int x2 = x + raio * cos(ang);
        int y2 = y - raio * sin(ang);

        // SDL_Rect point = {x2, y2, tam, tam};
        SDL_Point point = {x2, y2};
        colorirRect(&point, cor);
    }

}

int main(int argc, char* argv[]){


    if(!InitializeSDL()){
        printf("Erro na inicializacao do SDL\n");
        return 1;
    }

    int quadrado_size = 200;
                                //{x, y, largura (w), altura(h)}
    SDL_Rect quadrado_colorido = {0, 0, quadrado_size, quadrado_size};
    SDL_Texture* imagem_texture = load_image_to_texture("imagens/c_is_simple.jpeg");
    SDL_Rect quadrado_com_imagem = {(SCREEN_WIDTH - quadrado_size) / 2, (SCREEN_HEIGHT - quadrado_size) / 2, quadrado_size, quadrado_size};

    bool quit = false;

    int mouseX, mouseY; //coordenadas da posicao do mouse

    while(!quit){ //loop principal
        while(SDL_PollEvent(&event) != 0){ //loop que consome todos os eventos
            if(event.type == SDL_QUIT){
                quit = true; 
            }

            if(event.type == SDL_MOUSEBUTTONDOWN){
                SDL_GetMouseState(&mouseX, &mouseY);
                printf("clicou em (%d, %d)\n", mouseX, mouseY);
                quadrado_colorido.x = mouseX - quadrado_size/2;
                quadrado_colorido.y = mouseY - quadrado_size/2;
            }
        }        

        colorirFundo(verde);
        load_image_to_rect(imagem_texture, &quadrado_com_imagem);
        // colorirRect(&quadrado_colorido, vermelho);


        draw_circle(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 125, vermelho);
        draw_circle(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 75, azul);

        SDL_RenderPresent(renderer);
    }

    SDL_Quit();

    return 0;
}