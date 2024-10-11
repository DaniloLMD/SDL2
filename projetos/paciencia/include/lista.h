#ifndef __LISTA_H__
#define __LISTA_H__

#include "carta.h"

typedef struct node{ // Struct Nó de Lista
    carta *c; // Carta do Nó atual
    struct node *proximo; // Aponta para o próximo Nó da Lista
    struct node *anterior; // Aponta para o Nó anterior da Lista
} node;

typedef struct{ // Struct Lista
    node *first; // Aponta para o primeiro Nó da Lista
    int tamanho; // Tamanho da Lista
    node *last; // Aponta para o último Nó da Lista
    SDL_Rect base; // Rect da Lista (Coordenadas da sua base)
} lista;

#define QUANT_LISTAS 7 // Quantidade de Listas que serão usadas
lista listas[QUANT_LISTAS]; // vetor de Listas, que contem as sete "pilhas" de jogo (montes iniciais)

// Cabeçario de funções
void zerar_listas();
void insert(lista* l, carta *c);
void atualizar_listas();
carta* delete(lista *L, int indice);

#endif