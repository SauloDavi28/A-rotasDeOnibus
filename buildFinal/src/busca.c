#include <math.h>
#include <stdlib.h>
#include "busca.h"
#include "heap.h"

#define CUSTO_BALDEACAO 5.0f

/*
 * A busca nao trabalha so com paradas, e sim com ESTADOS (parada, linha):
 * "estou na parada P e cheguei nela pela linha L".
 *
 * Isso e necessario porque o custo do proximo trecho depende da linha em que
 * o passageiro esta (baldeacao custa CUSTO_BALDEACAO). Guardando uma unica
 * linha por parada, o algoritmo descarta caminhos que chegam na mesma parada
 * por outra linha com o mesmo custo (ou um pouco mais caro), mesmo quando
 * continuar nessa outra linha evitaria uma baldeacao logo adiante.
 *
 * Linha 0 significa "ainda nao embarcou" (usada so na origem).
 * Cada estado vira um unico inteiro para poder ser usado no Min-Heap.
 */
#define ESTADO(parada, linha) ((parada) * NUM_LINHAS + (linha))
#define PARADA_DE(estado)     ((estado) / NUM_LINHAS)
#define LINHA_DE(estado)      ((estado) % NUM_LINHAS)

/* Calcula a heuristica h(n): distancia em linha reta convertida para minutos. */
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

/*
 * Converte o caminho de estados (pai_estado[]) para os vetores por parada
 * que a interface usa: pai[p] = parada anterior a p na rota,
 * linha_usada[p] = linha com que se chegou em p (0 na origem).
 */
static void montar_rota(int estado_final, const int pai_estado[],
                        int pai[], int linha_usada[]) {
    int s = estado_final;

    while (s != -1) {
        int p = PARADA_DE(s);
        int anterior = pai_estado[s];

        linha_usada[p] = LINHA_DE(s);
        pai[p] = (anterior == -1) ? -1 : PARADA_DE(anterior);

        s = anterior;
    }
}

/*
 * Retorna 1 se encontrou rota, 0 se o destino e inalcancavel e -1 em caso
 * de erro (parametros invalidos ou falta de memoria).
 * Em caso de sucesso, pai[] e linha_usada[] descrevem a rota (de tras para
 * frente a partir do destino) e *tempo_total recebe o tempo em minutos.
 */
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

    int n = g->num_paradas;
    int num_estados = n * NUM_LINHAS;

    for (int i = 0; i < n; i++) {
        pai[i] = -1;
        linha_usada[i] = 0;
    }
    *tempo_total = INFINITY;

    /* Vetores indexados por estado (parada, linha). */
    float *g_score = malloc(num_estados * sizeof(float));
    float *f_score = malloc(num_estados * sizeof(float));
    int *pai_estado = malloc(num_estados * sizeof(int));
    MinHeap *aberto = criar_heap(num_estados);

    if (!g_score || !f_score || !pai_estado || !aberto) {
        free(g_score);
        free(f_score);
        free(pai_estado);
        liberar_heap(aberto);
        return -1;
    }

    /* No inicio, nenhum estado foi alcancado. */
    for (int s = 0; s < num_estados; s++) {
        g_score[s] = INFINITY;
        f_score[s] = INFINITY;
        pai_estado[s] = -1;
    }

    /* A origem comeca sem linha (0): o primeiro embarque nao e baldeacao. */
    int inicio = ESTADO(origem, 0);
    g_score[inicio] = 0.0f;
    f_score[inicio] = calcular_h(origem, destino);
    inserir_heap(aberto, inicio, f_score[inicio]);

    int resultado = 0;

    /* Continua enquanto houver estados abertos para explorar. */
    while (!heap_vazio(aberto)) {
        /* Escolhe o estado com a menor estimativa de custo total. */
        int atual = extrair_minimo(aberto);
        int parada_atual = PARADA_DE(atual);
        int linha_atual = LINHA_DE(atual);

        /* Chegou no destino (por qualquer linha): este e o menor custo. */
        if (parada_atual == destino) {
            *tempo_total = g_score[atual];
            montar_rota(atual, pai_estado, pai, linha_usada);
            resultado = 1;
            break;
        }

        /* Examina todos os trechos que saem da parada atual. */
        for (aresta *trecho = g->list_adj[parada_atual];
             trecho != NULL;
             trecho = trecho->prox) {
            int vizinho = trecho->destino;
            int linha = trecho->id_linha;

            /* Ignora trechos com destino ou linha invalidos. */
            if (vizinho < 0 || vizinho >= n || linha < 1 || linha >= NUM_LINHAS) {
                continue;
            }

            /* Soma o tempo do trecho ao custo acumulado ate o estado atual. */
            float novo_g = g_score[atual] + trecho->tempo_viagem;

            /* Troca de linha (ja estando em um onibus) custa a penalidade. */
            if (linha_atual != 0 && linha_atual != linha) {
                novo_g += CUSTO_BALDEACAO;
            }

            /* O proximo estado e (vizinho, linha deste trecho). */
            int proximo = ESTADO(vizinho, linha);

            /* Atualiza os dados somente se este caminho for mais barato. */
            if (novo_g < g_score[proximo]) {
                pai_estado[proximo] = atual;
                g_score[proximo] = novo_g;
                f_score[proximo] = novo_g + calcular_h(vizinho, destino);

                /* Insere o estado ou atualiza sua prioridade no Min-Heap. */
                inserir_heap(aberto, proximo, f_score[proximo]);
            }
        }
    }

    /* Se o heap esvaziou sem chegar ao destino, nao existe rota (resultado 0). */
    free(g_score);
    free(f_score);
    free(pai_estado);
    liberar_heap(aberto);
    return resultado;
}
