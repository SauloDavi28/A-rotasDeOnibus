#ifndef DADOS_H
#define DADOS_H

#include "grafo.h"

#define NUM_PARADAS 17

// id_linha das arestas (ficam sempre diferentes de 0)
#define LINHA_A 1
#define LINHA_B 2
#define LINHA_C 3
#define LINHA_D 4

// Quantidade de valores possiveis de linha: 0 (sem linha, na origem) + 4 linhas
#define NUM_LINHAS 5

extern float coord_x[NUM_PARADAS];          // km (convertido de lat/long)
extern float coord_y[NUM_PARADAS];          // km (convertido de lat/long)
extern const char* nome_parada[NUM_PARADAS];
extern const char* nome_linha[5];           // indexado pelo id_linha (1 a 4)
extern float velocidade_media;              // km/min, calculada em carregar_malha()

void carregar_malha(GRAFO* g);
float calcular_h(int parada_atual, int parada_destino);

#endif
