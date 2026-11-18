#ifndef GRAFO_H
#define GRAFO_H

// No = encontro de dois cmainhos = encontro de duas arestas
typedef struct aresta {
    int destino;                // indice do destino
    float tempo_viagem;         // Tempo da viagem, o peso da aresta
    int id_linha;        // Identificador da linha
    struct aresta* prox;        // Ponteiro para a próxima aresta da lista
}aresta;

typedef struct GRAFO {
    int num_paradas;            // Quantidade total de nos
    aresta** list_adj;// Array de ponteiros para as listas de adjacências
}GRAFO;

GRAFO* criar_grafo(int num_paradas);
void inserir_aresta(GRAFO* g, int origem, int destino, float tempo, int id_linha);
void liberar_grafo(GRAFO* g);

#endif