#ifndef __CARTA_H__
#define __CARTA_H__

#include "interface.h"

#define NAIPES 4 // quantidade de naipes
#define MAX 13 // quantidade de cartas de cada naipe 
#define TOTAL_CARTAS 52

const int altura_carta = 160; // Altura das cartas, para o rect; 
const int largura_carta = 105; // Largura das carta, para o rect;
const int divisao = 4; // Fração da carta que fica visível quando é tampada por outra (ex: 4 -> 1/4 da carta)

typedef struct{
    int numero; // A = 1, J = 11, Q = 12, K = 13
    int naipe;
    bool virada; // Se for true, virada pra baixo, se for false, virada pra cima
    SDL_Rect rect; // Coordenadas
} carta;

enum naipes{
    COPAS = 0, PAUS, OUROS, ESPADA // Enumera os naipes
};


// Variáveis do tipo Texture que armazenam a textura dos elementos
SDL_Texture* texture_cartas[NAIPES][MAX+1]; // Textura das cartas (primeira coluna nula, para identar o número das cartas apartir do 1)
SDL_Texture* texture_naipes[NAIPES]; // Textura do background das pilhas de guardar
SDL_Texture* texture_deck; // Textura do background 
SDL_Texture* texture_cartas_background; // Textura da parte de trás das cartas

char imagens_cartas_path[NAIPES][MAX+1][100] = { // Matriz de strings, que armazena o caminho ao jpg das cartas
    //LINHA 0: COPAS
    {"", "imagens/cartas/A_copas.png", "imagens/cartas/2_copas.png", "imagens/cartas/3_copas.png", "imagens/cartas/4_copas.png", "imagens/cartas/5_copas.png", "imagens/cartas/6_copas.png", "imagens/cartas/7_copas.png", "imagens/cartas/8_copas.png", "imagens/cartas/9_copas.png", "imagens/cartas/10_copas.png", "imagens/cartas/J_copas.png", "imagens/cartas/Q_copas.png", "imagens/cartas/K_copas.png"},
    //LINHA 1: PAUS
    {"", "imagens/cartas/A_paus.png", "imagens/cartas/2_paus.png", "imagens/cartas/3_paus.png", "imagens/cartas/4_paus.png", "imagens/cartas/5_paus.png", "imagens/cartas/6_paus.png", "imagens/cartas/7_paus.png", "imagens/cartas/8_paus.png", "imagens/cartas/9_paus.png", "imagens/cartas/10_paus.png", "imagens/cartas/J_paus.png", "imagens/cartas/Q_paus.png", "imagens/cartas/K_paus.png"},
    //LINHA 2: OUROS
    {"", "imagens/cartas/A_ouros.png", "imagens/cartas/2_ouros.png", "imagens/cartas/3_ouros.png", "imagens/cartas/4_ouros.png", "imagens/cartas/5_ouros.png", "imagens/cartas/6_ouros.png", "imagens/cartas/7_ouros.png", "imagens/cartas/8_ouros.png", "imagens/cartas/9_ouros.png", "imagens/cartas/10_ouros.png", "imagens/cartas/J_ouros.png", "imagens/cartas/Q_ouros.png", "imagens/cartas/K_ouros.png"},
    //LINHA 3: ESPADA
    {"", "imagens/cartas/A_espadas.png", "imagens/cartas/2_espadas.png", "imagens/cartas/3_espadas.png", "imagens/cartas/4_espadas.png", "imagens/cartas/5_espadas.png", "imagens/cartas/6_espadas.png", "imagens/cartas/7_espadas.png", "imagens/cartas/8_espadas.png", "imagens/cartas/9_espadas.png", "imagens/cartas/10_espadas.png", "imagens/cartas/J_espadas.png", "imagens/cartas/Q_espadas.png", "imagens/cartas/K_espadas.png"}
};

// Variaveis do tipo Rect, que armazenam as coordenadas
SDL_Rect naipes_rect[NAIPES]; // Rect das pilhas de guardar
SDL_Rect deck_rect[2]; // Rect dos decks de compra
SDL_Rect montes[7]; // Rect dos sete montes iniciais

char naipes_background_path[NAIPES][70] = { // Vetor de strings, que armazena o caminho ao jpg dos background das pilhas de guardar
    "imagens/fundos/copas_sprite.png",
    "imagens/fundos/paus_sprite.png",
    "imagens/fundos/ouros_sprite.png",
    "imagens/fundos/espada_sprite.png"
};

carta baralho[TOTAL_CARTAS]; // Vetor que armazena as cartas

// Cabeçario das funções
void iniciar_cartas();
void embaralhar();
void load_texture_naipes();
void load_texture_deck();
void load_texture_cartas();
void atualizar_background();
void distribuir_cartas();

#endif