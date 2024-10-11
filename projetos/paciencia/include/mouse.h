#ifndef __MOUSE_H__
#define __MOUSE_H__

#include "interface.h"
#include "lista.h"

int mouseX = 0, mouseY = 0; // Coordenadas do cursor
bool clicked = false; // inicializa o clique em falso (sem cliques)
lista mouse_list; // Lista de cartas que acompanha o cursor
int origem = -1; // Variável que armazena a origem das cartas que acompanham o cursor
// Origem: Deck de compras == 0 e 1; Pilhas de guardar == 2 ao 5; Montes de Jogar == 6 ao 12

// Cabeçario de funções
void zerar_mouse();
void atualizar_mouse();
lista* handle_first_click();
void handle_second_click();
void handle_click();

#endif