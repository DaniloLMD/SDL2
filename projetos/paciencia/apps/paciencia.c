#include "geral.h"

void limpa_memoria(){
    for(int i = 0; i < QUANT_LISTAS; i++){ // Liberar memória das listas
        while(listas[i].tamanho){
            delete(&listas[i], 0);
        }
    }

    while(mouse_list.tamanho){ // Liberar memória da mouse_list
        delete(&mouse_list, 0);
    }

    for(int i = 0; i < NAIPES; i++){ // Liberar memória das pilhas de guardar
        while(pilhas_g[i].tamanho){
            pop(&pilhas_g[i]);
        }
    }

    for(int i = 0; i < 2; i++){ // Liberar memória das pilhas de compra
        while(deck_pilha[i].tamanho){
            pop(&deck_pilha[i]);
        }
    }
}

void load_game(){
    limpa_memoria(); // Limpa a memória, se for necessário

    // Inicializando a textura de tudo
    load_texture_naipes();
    load_texture_deck();
    load_texture_cartas();
    background_texture = load_image_to_texture("imagens/fundos/background.jpg");
    texture_button_new_game = load_image_to_texture("imagens/fundos/new_game_button.jpg");
    texture_tela_vitoria = load_image_to_texture("imagens/fundos/winner.png");
    texture_exit_game = load_image_to_texture("imagens/fundos/exit_game.png");

    // Inicializando auxiliares para os rects
    const int border_distance_x = 20;
    const int border_distance_y = 15;
    const int distance_between_cards_y = 10;
    const int distance_between_cards_x = 30;
    int y = border_distance_y;
    int x = border_distance_x;
    int i = 0;

    // Carregando os rects do deck de compras
    for(i = 0; i < 2; i++){
        deck_rect[i].x = x;
        deck_rect[i].y = y;
        deck_rect[i].w = largura_carta;
        deck_rect[i].h = altura_carta;
        x += largura_carta + distance_between_cards_x;
    }

    // Carregando os rects das pilhas de guardar
    y += altura_carta + distance_between_cards_y + 28;
    for(int j = 0; j < NAIPES; j++, i++){
        naipes_rect[j].x = border_distance_x;
        naipes_rect[j].y = y;
        naipes_rect[j].w = largura_carta;
        naipes_rect[j].h = altura_carta;
        y += altura_carta + distance_between_cards_y;
    }

    // Carregando os rects da base dos montes
    x += largura_carta + 2*distance_between_cards_x;
    for(int i = 0; i < 7; i++){
        montes[i].x = x;
        montes[i].y = border_distance_y;
        montes[i].w = largura_carta;
        montes[i].h = altura_carta;
        x += distance_between_cards_x + largura_carta;
    }

    zerar_pilhas();
    zerar_listas();
    zerar_mouse();
    iniciar_cartas();
}

void update_all(){
    atualizar_background(); // Renderiza elementos estáticos
    atualizar_pilhas(); // Renderiza as cartas nas pilhas
    atualizar_listas(); // Renderiza as cartas nas listas (sete "pilhas" principais)
    atualizar_mouse(); // Renderiza as cartas do mouse
}

int main(){
    srand(time(NULL)); // Define a semente para rand()
    InitializeSDL(); // Inicializa a tela

    bool quit = false; // Inicializa quit em false

    load_game(); // Carrega a configuração incial do jogo

    while(!quit){ // Loop principal

        while(SDL_PollEvent(&event) != 0){ // Se houver interação
            if(event.type == SDL_QUIT){
                quit = true; // Se apertar sair, quit recebe true
            }
            if(event.type == SDL_MOUSEBUTTONDOWN){
                clicked = true; // Se for click, ativa o clicked
            }
        }

        update_all(); // Renderiza os elementos
                
        if(win_check()){ // Se o usuário vencer, apresenta a tela de vitória
            SDL_RenderCopy(renderer, texture_tela_vitoria, NULL, &vitoria); // Renderiza a tela de vitória
            SDL_Rect new_game_2 = {(SCREEN_WIDTH - 300) / 2, (SCREEN_HEIGHT) / 2 + 20, 300, 100};
            SDL_RenderCopy(renderer, texture_button_new_game, NULL, &new_game_2); // Renderiza um novo botão New Game, mais destacado
            SDL_Rect exit_game_2 = {(SCREEN_WIDTH - 300) / 2, (SCREEN_HEIGHT) / 2 + 140, 300, 100};
            SDL_RenderCopy(renderer, texture_exit_game, NULL, &exit_game_2); // Renderiza um novo botão Exit Game, mais destacado
            carta carta_fantasma; // Criando uma carta fantasma, que será inserida na lista do cursor
            carta_fantasma.numero = 13; // Fazendo com que o usuário não consiga mais pegar cartas na pilha
            insert(&mouse_list, &carta_fantasma);
            if (is_clicking_on_rect(&new_game_2)) { // Se o usuário clicar no botão New Game, reinicia o jogo
                load_game(); 
            }
            if (is_clicking_on_rect(&exit_game_2)){ // Se o usuário clicar no botão Exit Game, ele quita
                quit = true; 
            }         
        }

      
        if(clicked){ // Se houver click, verificar se é relevante
            if(is_clicking_on_rect(&new_game)){ // Se o usuário clicar no botão New Game, reinicia o jogo
                load_game();
            }
            if (is_clicking_on_rect(&exit_game)){ // Se o usuário clicar no botão Exit Game, ele quita
                quit = true; 
            }   
            handle_click();
            clicked = false; // Depois da verificação, clicked retorna para false
        }

        SDL_RenderPresent(renderer); // Mostra na tela os elementos renderizados pelo renderer
        SDL_RenderClear(renderer);

        SDL_Delay(2);
    }

    SDL_Quit();
    limpa_memoria();

    return 0;
}
