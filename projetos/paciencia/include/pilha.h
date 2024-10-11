#ifndef __PILHA_H__
#define __PILHA_H__

#include "carta.h"

typedef struct no{ // Struct Nó de Pilha
    carta *c; // Carta do Nó atual
    struct no* anterior; // Aponta para o Nó anterior
} no;

typedef struct{ // Struct Pilha
    int tamanho; // Tamanho da Pilha
    no *topo; // Aponta para o topo da Pilha
    SDL_Rect base; // Rect da Pilha (Coordenadas da sua base)
} pilha;

pilha pilhas_g[NAIPES]; // Vetor de Pilhas, que contem as quatro pilhas de guardar cartas
pilha deck_pilha[2]; // Vetor de Pilhas, que contem os deckes de compra

bool swap_deck_animation = false; // Animação de ir do deck 0 pro deck 1

// Cabeçario de funções
bool push(pilha *p, carta *c);
carta* pop(pilha *p);
void zerar_pilhas();
void atualizar_pilhas();

#endif