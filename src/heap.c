#include <stdio.h>
#include <stdlib.h>
#include "heap.h"

#define PAI(i) (((i) - 1) / 2)
#define ESQUERDO(i) (2 * (i) + 1)
#define DIREITO(i) (2 * (i) + 2)

typedef struct {
    int id_parada;
    float f_n;
} NoHeap;

struct MinHeap {
    NoHeap *nos;
    int *posicao_no_heap;
    int tamanho;
    int capacidade;
};

static void troca_nos(MinHeap *h, int i, int j) {
    NoHeap tmp = h->nos[i];
    h->nos[i] = h->nos[j];
    h->nos[j] = tmp;

    h->posicao_no_heap[h->nos[i].id_parada] = i;
    h->posicao_no_heap[h->nos[j].id_parada] = j;
}

static void subir(MinHeap *h, int i) {
    while (i > 0 && h->nos[i].f_n < h->nos[PAI(i)].f_n) {
        troca_nos(h, i, PAI(i));
        i = PAI(i);
    }
}

MinHeap *criar_heap(int num_paradas) {
    MinHeap *h = malloc(sizeof(MinHeap));
    if (!h) return NULL;

    h->nos = malloc(num_paradas * sizeof(NoHeap));
    h->posicao_no_heap = malloc(num_paradas * sizeof(int));

    if (!h->nos || !h->posicao_no_heap) {
        free(h->nos);
        free(h->posicao_no_heap);
        free(h);
        
        return NULL;
    }

    for (int i = 0; i < num_paradas; i++) {
        h->posicao_no_heap[i] = -1;
    }

    h->tamanho = 0;
    h->capacidade = num_paradas;

    return h;
}

static int id_valido(MinHeap *h, int id_parada) {
    if (id_parada < 0 || id_parada >= h->capacidade) {
        fprintf(stderr, "Erro: id_parada %d fora do intervalo [0, %d]\n", id_parada, h->capacidade - 1);
        return 0;
    }

    return 1;
}

void liberar_heap(MinHeap *h) {
    if (!h) return;

    free(h->nos);
    free(h->posicao_no_heap);
    free(h);
}

int heap_vazio(MinHeap *h) {
    return h->tamanho == 0;
}

int heap_contem(MinHeap *h, int id_parada) {
    if (!id_valido(h, id_parada)) return 0;

    return h->posicao_no_heap[id_parada] !=  -1;
}

static void heapify(MinHeap *h, int i) {
    while (1) {
        int menor = i;
        int esq = ESQUERDO(i);
        int dir = DIREITO(i);

        if (esq < h->tamanho && h->nos[esq].f_n < h->nos[menor].f_n) menor = esq;

        if (dir < h->tamanho && h->nos[dir].f_n < h->nos[menor].f_n) menor = dir;

        if (menor == i) break;

        troca_nos(h, i, menor);
        i = menor;
    }
}

void diminuir_chave(MinHeap *h, int id_parada, float novo_f) {
    if (!id_valido(h, id_parada)) return;

    int i = h->posicao_no_heap[id_parada];

    if (i == -1 || novo_f >= h->nos[i].f_n) return;

    h->nos[i].f_n = novo_f;
    subir(h, i);
}

void inserir_heap(MinHeap *h, int id_parada, float f_n) {
    if (!id_valido(h, id_parada)) return;

    if (heap_contem(h, id_parada)) {
        diminuir_chave(h, id_parada, f_n);
        return;
    }

    if (h->tamanho == h->capacidade) {
        fprintf(stderr, "Erro: heap cheio\n");
        return;
    }

    int i = h->tamanho;
    h->nos[i].id_parada = id_parada;
    h->nos[i].f_n = f_n;
    h->posicao_no_heap[id_parada] = i;
    h->tamanho++;

    subir(h, i);
}

int extrair_minimo(MinHeap *h) {
    if (heap_vazio(h)) return -1;

    int minimo = h->nos[0].id_parada;

    troca_nos(h, 0, h->tamanho - 1);

    h->tamanho--;
    h->posicao_no_heap[minimo] = -1;

    if (h->tamanho > 0) heapify(h, 0);

    return minimo;
}