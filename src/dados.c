#include "dados.h"

// Coordenadas em km (mapa ficticio)
float coord_x[NUM_PARADAS] = {0, 1, 3, 5, 6, 8, 9, 5, 8, 3};
float coord_y[NUM_PARADAS] = {0, 1, 1, 1, 3, 3, 5, 4, 6, 4};

const char* nome_parada[NUM_PARADAS] = {
    "Terminal Petrolina",   // 0
    "Orla Petrolina",       // 1
    "Ponte",                // 2
    "Terminal Juazeiro",    // 3
    "Centro Juazeiro",      // 4
    "Hospital",             // 5
    "Campus Univasf",       // 6
    "Rodoviaria",           // 7
    "Portaria Univasf",     // 8
    "Bairro Tabuleiro"      // 9
};

// Insere o trecho nos dois sentidos (o onibus vai e volta)
static void trecho(GRAFO* g, int a, int b, float tempo, int linha) {
    inserir_aresta(g, a, b, tempo, linha);
    inserir_aresta(g, b, a, tempo, linha);
}

void carregar_malha(GRAFO* g) {
    // Linha 101: Terminal Petrolina -> Campus Univasf (pela ponte)
    trecho(g, 0, 1, 4.0, 101);
    trecho(g, 1, 2, 5.0, 101);
    trecho(g, 2, 3, 5.0, 101);
    trecho(g, 3, 4, 6.0, 101);
    trecho(g, 4, 5, 5.0, 101);
    trecho(g, 5, 6, 6.0, 101);

    // Linha 202: Terminal Juazeiro -> Rodoviaria -> Portaria -> Campus
    trecho(g, 3, 7, 7.0, 202);
    trecho(g, 7, 8, 8.0, 202);
    trecho(g, 8, 6, 4.0, 202);

    // Linha 303: Orla -> Tabuleiro -> Rodoviaria -> Centro Juazeiro
    trecho(g, 1, 9, 8.0, 303);
    trecho(g, 9, 7, 5.0, 303);
    trecho(g, 7, 4, 4.0, 303);
}
