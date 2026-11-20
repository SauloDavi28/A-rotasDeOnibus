#include <math.h>
#include <stddef.h>
#include "busca.h"
#include "heap.h"


#define CUSTO_BALDEACAO 5.0f

/* Calcula a heurística h(n): distância em linha reta convertida para minutos. */
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

int busca_A_estrela(
    GRAFO *g,
    int origem,
    int destino,
    int pai[],
    int linha_usada[],
    float *tempo_total
) {
    /* Confere as entradas antes de acessar o grafo e os vetores. */
    if (!g || !g->list_adj || !pai || !linha_usada || !tempo_total ||
        g->num_paradas <= 0 ||
        origem < 0 || origem >= g->num_paradas ||
        destino < 0 || destino >= g->num_paradas) {
        return -1;
    }

    /* Cada posição dos vetores representa uma parada do grafo. */
    int n = g->num_paradas;
    float g_score[n];
    float f_score[n];

    /* No início, nenhuma parada foi alcançada. */
    for (int i = 0; i < n; i++) {
        g_score[i] = INFINITY;
        f_score[i] = INFINITY;
        pai[i] = -1;
        linha_usada[i] = -1;
    }

    /* A origem começa com custo zero; sua prioridade é apenas a heurística. */
    g_score[origem] = 0.0f;
    f_score[origem] = calcular_h(origem, destino);
    *tempo_total = INFINITY;

    /* O heap mantém as paradas abertas em ordem crescente de f(n). */
    MinHeap *aberto = criar_heap(n);
    if (!aberto) {
        return -1;
    }

    inserir_heap(aberto, origem, f_score[origem]);

    /* Continua enquanto houver paradas abertas para explorar. */
    while (!heap_vazio(aberto)) {
        /* Escolhe a parada com a menor estimativa de custo total. */
        int atual = extrair_minimo(aberto);

        /* Quando o destino é escolhido, o menor custo da rota foi encontrado. */
        if (atual == destino) {
            *tempo_total = g_score[destino];
            liberar_heap(aberto);
            return 1;
        }

        /* Examina todos os trechos que saem da parada atual. */
        for (aresta *trecho = g->list_adj[atual];
             trecho != NULL;
             trecho = trecho->prox) {
            int vizinho = trecho->destino;

            /* Ignora um destino inválido na lista de adjacências. */
            if (vizinho < 0 || vizinho >= n) {
                continue;
            }

            /* Soma o tempo do trecho ao custo acumulado até a parada atual. */
            float novo_g = g_score[atual] + trecho->tempo_viagem;

            /* Adiciona cinco minutos quando é necessário trocar de linha. */
            if (linha_usada[atual] != -1 &&
                linha_usada[atual] != trecho->id_linha) {
                novo_g += CUSTO_BALDEACAO;
            }

            /* Atualiza os dados somente se este caminho for mais barato. */
            if (novo_g < g_score[vizinho]) {
                pai[vizinho] = atual;
                linha_usada[vizinho] = trecho->id_linha;
                g_score[vizinho] = novo_g;
                f_score[vizinho] = novo_g + calcular_h(vizinho, destino);

                /* Insere a parada ou atualiza sua prioridade no Min-Heap. */
                inserir_heap(aberto, vizinho, f_score[vizinho]);
            }
        }
    }

    /* Heap vazio significa que não existe rota para o destino. */
    liberar_heap(aberto);
    return 0;
}