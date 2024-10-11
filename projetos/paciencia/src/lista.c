#include "geral.h"

void zerar_listas(){ // Zera as Listas
    for(int k = 0; k < QUANT_LISTAS; k++){
        listas[k].first = NULL;
        listas[k].last = NULL;
        listas[k].tamanho = 0;
        listas[k].base = montes[k];
    }
}

void insert(lista* l, carta *c){ // Inserir carta na Lista 
    if(!l || !c){
        printf("insert error: invalid paramater\n");
    }
    
    node *novo_node = (node *)malloc(sizeof(node));
    if(!novo_node){
        printf("insert error: bad_alloc\n");
    }
    
    novo_node->proximo = NULL;
    novo_node->c = c;
    novo_node->anterior = l->last;
    
    if(l->tamanho == 0){
        l->first = novo_node;
    }
    else{
        l->last->proximo = novo_node;
    }
    
    l->last = novo_node;
    l->tamanho++;
    c->rect.x = l->base.x;
    c->rect.y = l->base.y;
}

void atualizar_listas(){ // Percorre as listas, e as renderiza constantemente    
    for(int i = 0; i < QUANT_LISTAS; i++){
        if(listas[i].tamanho == 0) continue; // Se não houver cartas na lista, vai para a próxima
        node* at = listas[i].first; // auxiliar, para percorrer as cartas na lista

        int renderizadas = 0;
        while(at){ // Enquanto houver cartas na lista
            int naipe = at->c->naipe; // informações da carta atual
            int numero = at->c->numero;

            int variacao_y = renderizadas * altura_carta / divisao; // Variação da altura das cartas sobrepostas
            at->c->rect.y = listas[i].base.y + variacao_y;

            if(at == listas[i].last && mouse_list.tamanho == 0) at->c->virada = false; // Se a carta estiver de costas e não houver cartas no mouse, desvira ela
            SDL_Texture* t = (at->c->virada) ? texture_cartas_background : texture_cartas[naipe][numero]; // Carrega a textura da carta (de frente ou de costas) 
            SDL_RenderCopy(renderer, t, NULL, &at->c->rect); // Renderiza a carta
            
            at = at->proximo; 
            renderizadas++;
        }
    }
}

carta* delete(lista *L, int indice){ // Remove carta da Lista
    if(!L){
        printf("remove error: invalid list\n");
        return NULL;
    }
    if(indice > L->tamanho){
        printf("remove error: invalid index\n");
        return NULL;
    }
    if(L->tamanho == 0){
        printf("remove error: empty list\n");
        return NULL;
    }

    node* at = L->first;
    for(int indice_at = 0; indice_at != indice; indice_at++){
        at = at->proximo;
    }

    if(at == L->first){
        L->first = L->first->proximo;
    }
    if(at == L->last){
        L->last = L->last->anterior;
    }

    if(at->proximo){
        at->proximo->anterior = at->anterior;
    }
    if(at->anterior){
        at->anterior->proximo = at->proximo;
    }
    
    L->tamanho--;
    carta* c = at->c;
    free(at);
    return c; // Retorna a carta removida, para ser usada no "push" em outra "pilha"
}