
#include "geral.h"

void zerar_mouse(){ // Zera os elementos do cursor
    mouseX = 0;
    mouseY = 0;
    mouse_list.first = mouse_list.last = NULL;
    mouse_list.tamanho = 0;    
}

void atualizar_mouse(){ // Atualiza/Renderiza os elementos do cursor
    SDL_GetMouseState(&mouseX, &mouseY); // Atualiza as coordenadas do cursor
    mouse_list.base.x = mouseX; // Atualiza o rect da base da Lista do cursor, com base no local atual do cursor
    mouse_list.base.y = mouseY;

    int renderizadas = 0;
    for(node* at = mouse_list.first; at; at = at->proximo){ // Renderiza as cartas da lista do mouse
        at->c->rect.x = mouseX - largura_carta/2;

        int variacao_y = renderizadas * altura_carta / divisao;
        at->c->rect.y = mouse_list.base.y + variacao_y;

        int numero = at->c->numero;
        int naipe = at->c->naipe;
        SDL_RenderCopy(renderer, texture_cartas[naipe][numero], NULL, &at->c->rect);
        renderizadas++;
    }
}

lista* handle_first_click(){ // Verificações do primeiro click
    if(mouse_list.tamanho) return NULL;

    // Checando se esta clicando em um dos sete montes
    for(int i = 0; i < QUANT_LISTAS; i++){
        if(!listas[i].tamanho) continue; // Se não houver cartas na Lista, vai para a próxima
        node* at = listas[i].last;
        for(int j = listas[i].tamanho - 1; j >= 0 && at != NULL; j--){
            if(is_clicking_on_rect(&at->c->rect) && !at->c->virada){ // Verdadeiro apenas se clicar no rect de uma carta desvirada
                origem = 6 + i; // Armazena a Lista em que foi pega a(s) carta(s)
                int qtd = listas[i].tamanho - j;
                while(qtd--){ // Adiciona a(s) carta(s) selecionada(s) na Lista do mouse
                    insert(&mouse_list, delete(&listas[i], j)); 
                }
                i = QUANT_LISTAS;
                return &mouse_list; // Retorna a(s) carta(s) selecionada(s) pelo usuário
            }
            at = at->anterior;
        }
    }

    // Checando se esta clicando no deck de compras (de cartas viradas)
    if(is_clicking_on_rect(&deck_pilha[0].base)){
        if(deck_pilha[0].tamanho) swap_deck_animation = true; // Se houver cartas no deck de compras, ativa a animação 
        else{ // Se não houver cartas no deck de compras, reinicia o deck
            while(deck_pilha[1].tamanho){   
                push(&deck_pilha[0], pop(&deck_pilha[1]));
                deck_pilha[0].topo->c->virada = true;
            }
        }
        return NULL;
    }

    // Checando se esta clicando no deck de compras (de cartas desviradas)
    if(is_clicking_on_rect(&deck_pilha[1].base) && deck_pilha[1].tamanho){
        origem = 1;
        insert(&mouse_list, pop(&deck_pilha[1])); // Adiciona a carta clicada na Lista do cursor
        return &mouse_list; // Retorna a(s) carta(s) selecionada(s) pelo usuário
    }
    
    // Checando se esta clicando nas pilhas de guardar
    for(int i = 0; i < NAIPES; i++){
        if(is_clicking_on_rect(&naipes_rect[i]) && pilhas_g[i].tamanho){
            origem = 2 + i;
            insert(&mouse_list, pop(&pilhas_g[i])); // Adiciona a carta clicada na Lista do cursor
            return &mouse_list; // Retorna a(s) carta(s) selecionada(s) pelo usuário
        }
    }

    return NULL; 
}

void handle_second_click(){ // Verificações do segundo click
    if(mouse_list.tamanho == 0) return; // "Ativa" a função apenas se houver cartas na Lista do cursor 
    if(origem == -1) return;

    // Checando se esta clicando em um dos sete monte
    for(int i = 0; i < QUANT_LISTAS; i++){       
        SDL_Rect gambiarra = (listas[i].tamanho) ?  listas[i].last->c->rect : listas[i].base; 
        if(is_clicking_on_rect(&gambiarra)){ // Se houver cartas no Monte clicado, verifica se clicou no rect da primeira (de cima pra baixo), se não, verifica se clicou no rect da Lista 
            if(verifica_lista(&mouse_list, &listas[i])){  // Chamar a Verificar com a(s) carta(s) retornada(s) por click1 e a lista clicada agora 
                while(mouse_list.tamanho){ // Se for válido, inserir as cartas da Lista do cursor no Monte
                    insert(&listas[i], delete(&mouse_list, 0));
                } return;
            }  else{ // Se não for válido devolver a(s) carta(s) da Lista do cursor para sua Origem (desfazer movimento)
                    if(origem >= 6){
                        while(mouse_list.tamanho){
                            insert(&listas[origem - 6], delete(&mouse_list, 0));
                        }
                        return;    
                    }
                    else if(origem >= 2){
                        push(&pilhas_g[origem - 2], delete(&mouse_list, 0));
                        return;
                    }
                    else{
                        push(&deck_pilha[1], delete(&mouse_list, 0));
                        return;
                    }              
            }
        }
    }

    // Checando se esta clicando nas pilhas de guardar
    for(int i = 0; i < NAIPES && mouse_list.tamanho == 1; i++){
        if(is_clicking_on_rect(&naipes_rect[i])){ // Verifica se clicou no rect da Pilha de guardar
            if(verifica_pilha(&mouse_list, &pilhas_g[i], i)){  // Chamar a Verificar com a(s) carta(s) retornada(s) por click1 e pilha clicada agora 
                while(mouse_list.tamanho){ // Se for válido, guardar carta do cursor na pilha
                    push(&pilhas_g[i], delete(&mouse_list, 0));
                } return;
            }  else{ // Se não for válido devolver a(s) carta(s) da Lista do cursor para sua Origem (desfazer movimento)
                    if(origem >= 6){
                        while(mouse_list.tamanho){
                            insert(&listas[origem - 6], delete(&mouse_list, 0));
                        }
                        return;    
                    }
                    else if(origem >= 2){
                        push(&pilhas_g[origem - 2], delete(&mouse_list, 0));
                        return;
                    }
                    else{
                        push(&deck_pilha[1], delete(&mouse_list, 0));
                        return;
                    }              
            }
        }
    }
    return;
}

void handle_click(){
    if(!mouse_list.tamanho){ // Se não houver carta(s) na lista do mouse, é o primeiro clique
        handle_first_click(); 
    }
    else{ // Se houver carta(s) na lista do mouse, é o segundo clique
        handle_second_click();
    }    
}
