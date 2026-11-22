#include <stdio.h>
#include <stdlib.h>
#include "grafo.h"

GRAFO* criar_grafo(int num_paradas){
    GRAFO *grafo = (GRAFO*) malloc(sizeof(GRAFO));
    if(!grafo) return NULL;

    grafo->num_paradas = num_paradas;
    
    grafo->list_adj = (aresta **) malloc(num_paradas*sizeof(aresta*));
    if(!grafo->list_adj){
        free(grafo);
        return NULL;
    }

    for(int i = 0; i < num_paradas; i++){
        grafo->list_adj[i] = NULL;
    }

    return grafo;

}

void inserir_aresta(GRAFO* grafo, int origem, int destino, float tempo, int id_linha){

    aresta *nova_aresta = (aresta *) malloc(sizeof(aresta));
    if(!nova_aresta)return;

    nova_aresta->destino = destino;
    nova_aresta->tempo_viagem = tempo;
    nova_aresta->id_linha = id_linha;

    nova_aresta->prox = grafo->list_adj[origem]; //o proximo de nova_aresta recebe o endereco atual do primeiro elemento, que se nao foi preenchido continua como NULL
    grafo->list_adj[origem] = nova_aresta; //o inicio do grafo agora eh nova aresta, o grafo cresce de cima pra baixo

}

void liberar_grafo(GRAFO* grafo){

    if(!grafo)return;

    //libera a lista encadeada de cada parada
    for(int i = 0; i < grafo->num_paradas; i++){
        aresta *atual = grafo->list_adj[i];
        while(atual != NULL){
            aresta *temp = atual;
            atual = atual->prox;
            free(temp);
        }
    }
    free(grafo->list_adj);
    free(grafo);
}
