#include "../include/sdl_manager.h"
#include "../include/cor.h"

void swap(int* a, int* b){
    int t = *a;
    *a = *b;
    *b = t;
}

/*
    Y - y0 = m * (X - x0)
    Y = m * (X - x0) + y0
    X = (Y - y0) / M    - x0
*/
void draw_line(int x1, int y1, int x2, int y2, Cor cor){
    if(x1 > x2){
        swap(&x1, &x2);
        swap(&y1, &y2);
    }

    int dx = x2 - x1;
    int dy = y2 - y1;

    double m = (double) dy / dx;

    SDL_Rect pixel = {0, 0, 1,1};

    for(int x = x1; x <= x2; x++){
        int y = m * (x - x1) + y1;
        pixel.x = x;
        pixel.y = y;
        colorirRect(&pixel, cor);
    }
}

#define PI acos(-1)
void draw_circle(int x, int y, int r, Cor cor){
    double ang = 0;
    int pontos = 1000;
    double var = 2 * PI / pontos;

    SDL_Rect pixel = {0, 0, 1, 1};
    for(int i = 0; i < pontos; i++, ang += var){
        pixel.x = x + r * cos(ang);
        pixel.y = y - r * sin(ang);

        colorirRect(&pixel, cor);

        SDL_Rect inverso = {0, 0, 1, 1};
        double ang2 = ang + (PI);
        inverso.x = x + r * cos(ang2);
        inverso.y = y + r * sin(ang2);


        SDL_RenderDrawLine(renderer, pixel.x, pixel.y, inverso.x, inverso.y);
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
        colorirRect(&quadrado_colorido, vermelho);

        draw_line(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, preto);

        draw_circle(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 150, vermelho);


        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderDrawLine(renderer, SCREEN_WIDTH/2, 0, SCREEN_WIDTH/2, SCREEN_HEIGHT);


        SDL_RenderPresent(renderer);
    }

    SDL_Quit();

    return 0;
}