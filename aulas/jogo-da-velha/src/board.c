#include "../include/board.h"
#include "../include/cor.h"

void board_initialize(){
    //inicializando o tabuleiro
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            SDL_Rect atual = {j * SCREEN_WIDTH/3, i * SCREEN_HEIGHT/3 ,SCREEN_WIDTH/3, SCREEN_HEIGHT/3};
            board[i][j] = atual;
            board_textures[i][j] = NULL;    

            SDL_Rect atual2 = {(2 * atual.x + atual.w - board_images_rect_size)/2 , (2 * atual.y + atual.h - board_images_rect_size)/2, board_images_rect_size, board_images_rect_size};
            board_images[i][j] = atual2;
        }
    }

    x_texture = load_image_to_texture("imagens/x.png");
    o_texture = load_image_to_texture("imagens/o.png");
}

void draw_board_lines(){
    Cor linha_cor = preto;

    int linha_width = 5;
    
    SDL_Rect linha_vertical_1 = {SCREEN_WIDTH/3, 0, linha_width, SCREEN_HEIGHT};
    SDL_Rect linha_vertical_2 = {2 * SCREEN_WIDTH/3, 0, linha_width, SCREEN_HEIGHT};

    SDL_Rect linha_horizontal_1 = {0, SCREEN_HEIGHT/3, SCREEN_WIDTH, linha_width};
    SDL_Rect linha_horizontal_2 = {0, 2 * SCREEN_HEIGHT/3, SCREEN_WIDTH, linha_width};
    
    colorirRect(&linha_vertical_1, linha_cor);
    colorirRect(&linha_vertical_2, linha_cor);

    colorirRect(&linha_horizontal_1, linha_cor);
    colorirRect(&linha_horizontal_2, linha_cor);
}

void draw_board(){

    //desenhando o tabuleiro (matriz 3x3)
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            colorirRect(&board[i][j], verde);

        }
    }

    //desenhando as linhas de separacao
    draw_board_lines();

    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            if(board_textures[i][j] != NULL){
                load_image_to_rect(board_textures[i][j], &board_images[i][j]);
            }   
        }
    }
}

int clicks = 0;
void handle_click(){
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);   

    SDL_Rect mouse_rect = {mouseX, mouseY, 1, 1};

    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            if(check_colision(&mouse_rect, &board[i][j]) && board_textures[i][j] == NULL){
                if(clicks%2 == 0) board_textures[i][j] = x_texture;
                else board_textures[i][j] = o_texture;
                clicks++;
                return;
            }
        }
    } 

}