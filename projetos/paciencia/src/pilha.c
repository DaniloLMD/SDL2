#include "geral.h"

void zerar_pilhas(){ // Zera as pilhas
    
    for(int i = 0; i < NAIPES; i++){ // Pilhas de guardar carta 
        pilhas_g[i].tamanho = 0;
        pilhas_g[i].topo = NULL;
        pilhas_g[i].base = naipes_rect[i];
    }

    for(int i = 0; i < 2; i++){ // Pilhas do deck de compra
        deck_pilha[i].tamanho = 0;
        deck_pilha[i].topo = NULL;
        deck_pilha[i].base = deck_rect[i];
    }
}

bool push(pilha *p, carta *c){ // Adiciona uma carta no topo da Pilha
    no *novo;
    novo = (no*)malloc(sizeof(no));
    if(novo == NULL){
        printf("erro no push da pilha\n");
        return false;
    }

    c->rect.x = p->base.x;
    c->rect.y = p->base.y;
    novo->c = c;
    novo->anterior = p->topo;
    p->topo = novo;
    p->tamanho++;

    return true;
}

carta* pop(pilha *p){ // Remove uma carta do topo da pilha
    if(p->tamanho == 0){
        printf("pilha vazia\n");
        return NULL;
    }

    no *aux = p->topo;
    carta* c = aux->c;
    
    p->topo = p->topo->anterior;
    p->tamanho--;
    free(aux);

    return c; // Retorna a carta removida, para ser usada no "push" em outra "pilha"
}

void atualizar_pilhas(){ // Função que renderiza as pilhas constantemente    
    
    if(swap_deck_animation){ 
        if(deck_pilha[0].tamanho > 1){ // Se houver mais de uma carta na pilha de compras, renderizar a parte de trás das cartas
            SDL_RenderCopy(renderer, texture_cartas_background, NULL, &deck_pilha[0].topo->anterior->c->rect);
        }

        if(deck_pilha[0].topo->c->rect.x <= deck_pilha[1].base.x){ // Animação do movimento da carta
            deck_pilha[0].topo->c->rect.x++; // Varia o rect da carta do topo, até ela atingir o topo do outro deck      
        }
        else{  // Quando o rect da carta se igualar ao rect do outro deck             
            push(&deck_pilha[1], pop(&deck_pilha[0])); // Insierir a carta na Pilha do deck
            deck_pilha[1].topo->c->virada = false; // Desvirar a carta
            swap_deck_animation = false; // Volta a ser verdadeiro se o usuário tentar comprar (e for válido)
        }
    }

    // Renderizando as Pilhas do deck
    for(int i = 1; i >= 0; i--){
        if(deck_pilha[i].tamanho == 0) continue;
        SDL_Texture* t = texture_cartas[deck_pilha[i].topo->c->naipe][deck_pilha[i].topo->c->numero];
        if(deck_pilha[i].topo->c->virada){
            t = texture_cartas_background;
        }
        SDL_RenderCopy(renderer, t,NULL, &deck_pilha[i].topo->c->rect);
    }

    // Renderizando as Pilhas de guardar
    for(int i = 0; i < NAIPES; i++){
        if(pilhas_g[i].topo == NULL) continue;
        SDL_RenderCopy(renderer, texture_cartas[pilhas_g[i].topo->c->naipe][pilhas_g[i].topo->c->numero], NULL, &pilhas_g[i].topo->c->rect);       
    }
}
