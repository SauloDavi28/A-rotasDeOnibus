#ifndef DADOS_H
#define DADOS_H

#include "grafo.h"

#define NUM_PARADAS 10

extern float coord_x[NUM_PARADAS];
extern float coord_y[NUM_PARADAS];
extern const char* nome_parada[NUM_PARADAS];

void carregar_malha(GRAFO* g);

#endif
