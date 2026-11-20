#include <math.h>
#include "dados.h"

/* Calcula a heuristica h(n): distancia euclidiana em linha reta
   convertida para tempo estimado (minutos) */
float calcular_h(int parada_atual, int parada_destino) {
    if (parada_atual < 0 || parada_atual >= NUM_PARADAS ||
        parada_destino < 0 || parada_destino >= NUM_PARADAS) {
        return 0.0f;
    }

    if (parada_atual == parada_destino || velocidade_media <= 0.0f) {
        return 0.0f;
    }

    float dx = coord_x[parada_destino] - coord_x[parada_atual];
    float dy = coord_y[parada_destino] - coord_y[parada_atual];
    float distancia = sqrtf(dx * dx + dy * dy);

    return distancia / velocidade_media;
}
