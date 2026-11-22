#ifndef HEAP_H
#define HEAP_H

typedef struct MinHeap MinHeap;

MinHeap *criar_heap(int num_paradas);

void liberar_heap(MinHeap *h);

int heap_vazio(MinHeap *h);

int heap_contem(MinHeap *h, int id_parada);

void inserir_heap(MinHeap *h, int id_parada, float f_n);

int extrair_minimo(MinHeap *h);

void diminuir_chave(MinHeap *h, int id_parada, float novo_f);

#endif