#ifndef BUSCA_H
#define BUSCA_H

#include "dados.h"

float calcular_h(int parada_atual, int parada_destino);

int busca_A_estrela(
    GRAFO *g,
    int origem,
    int destino,
    int pai[],
    int linha_usada[],
    float *tempo_total
);




#endif
