#include <math.h>
#include "dados.h"

#define PI 3.14159265f

static const float lat[NUM_PARADAS] = {
    -9.412673720400798f,  // 0  Univasf Campus Juazeiro
    -9.415332785322489f,  // 1  Terminal de Juazeiro (Camelodromo)
    -9.389648859169695f,  // 2  Univasf Campus Petrolina
    -9.385122380591573f,  // 3  Sementeira (Av. da Integracao)
    -9.388416628732136f,  // 4  Petrape (Av. da Integracao)
    -9.364035281397888f,  // 5  Plante Bem
    -9.385445500386941f,  // 6  Feira da Cohab Massangano
    -9.362148658175421f,  // 7  BR 407 - Izabela Construcoes
    -9.32180f,            // 8  Residencia Estudantil CCA
    -9.32389f,            // 9  CCA - Bloco Antigo
    -9.33066f,            // 10 CCA - Hospital Veterinario
    -9.32480f,            // 11 CCA - Bloco de Salas de Aula
    -9.389972017833120f,  // 12 G Barbosa Petrolina
    -9.376072582074487f,  // 13 Canteiro de Obras (Av. 7 de Setembro)
    -9.385654241724195f,  // 14 Posto Sao Francisco (Av. 7 de Setembro)
    -9.389333057631240f,  // 15 Supermercado Regente
    -9.370991740943255f   // 16 Nutiva Agro (Av. Transnordestina)
};

static const float lon[NUM_PARADAS] = {
    -40.51522711743834f,  // 0
    -40.50514898221689f,  // 1
    -40.49561316103118f,  // 2
    -40.50941085520911f,  // 3
    -40.51786635918067f,  // 4
    -40.48219946677314f,  // 5
    -40.52883266641953f,  // 6
    -40.54585034568906f,  // 7
    -40.56120f,           // 8
    -40.56000f,           // 9
    -40.55775f,           // 10
    -40.55930f,           // 11
    -40.48873296617766f,  // 12
    -40.49517754859295f,  // 13
    -40.52307216490445f,  // 14
    -40.50900102888163f,  // 15
    -40.53387533503611f   // 16
};

float coord_x[NUM_PARADAS];
float coord_y[NUM_PARADAS];
float velocidade_media = 0.963f; // recalculada dinamicamente em carregar_malha()

const char* nome_parada[NUM_PARADAS] = {
    "Univasf Campus Juazeiro",          // 0
    "Terminal de Juazeiro",             // 1
    "Univasf Campus Petrolina",         // 2
    "Sementeira",                       // 3
    "Petrape",                          // 4
    "Plante Bem",                       // 5
    "Feira da Cohab Massangano",        // 6
    "BR 407 - Izabela Construcoes",     // 7
    "Residencia Estudantil CCA",        // 8
    "CCA - Bloco Antigo",               // 9
    "CCA - Hospital Veterinario",       // 10
    "CCA - Bloco de Salas de Aula",     // 11
    "G Barbosa Petrolina",              // 12
    "Canteiro de Obras (Av. 7 Set.)",   // 13
    "Posto Sao Francisco (Av. 7 Set.)", // 14
    "Supermercado Regente",             // 15
    "Nutiva Agro (Transnordestina)"     // 16
};

const char* nome_linha[5] = {
    "-", "Onibus A", "Onibus B", "Onibus C", "Onibus D"
};

static int coordenadas_convertidas = 0;

/* Converte lat/long (graus) para X, Y em km, tendo a parada 0 como origem */
static void converter_coordenadas(void) {
    float lat0 = lat[0] * PI / 180.0f;
    for (int i = 0; i < NUM_PARADAS; i++) {
        coord_x[i] = (lon[i] - lon[0]) * 111.32f * cosf(lat0);
        coord_y[i] = (lat[i] - lat[0]) * 110.57f;
    }
    coordenadas_convertidas = 1;
}

static float vel_max; // maior velocidade (km/min) entre todas as arestas

/* Insere o trecho nos dois sentidos e atualiza a maior velocidade do mapa */
static void trecho(GRAFO* g, int a, int b, float tempo, int linha) {
    inserir_aresta(g, a, b, tempo, linha);
    inserir_aresta(g, b, a, tempo, linha);

    float dx = coord_x[b] - coord_x[a];
    float dy = coord_y[b] - coord_y[a];
    float v = sqrtf(dx * dx + dy * dy) / tempo;
    if (v > vel_max) vel_max = v;
}

void carregar_malha(GRAFO* g) {
    converter_coordenadas();
    vel_max = 0.0f;

    // Onibus A: Terminal -> Petrolina -> Regente -> Cohab -> BR 407 -> CCA
    trecho(g, 1, 2, 5.0f, LINHA_A);
    trecho(g, 2, 15, 5.0f, LINHA_A);
    trecho(g, 15, 6, 8.0f, LINHA_A);
    trecho(g, 6, 7, 5.0f, LINHA_A);
    trecho(g, 7, 10, 5.0f, LINHA_A);

    // Onibus B: Juazeiro -> Petrolina -> Av. da Integracao -> Cohab -> CCA
    trecho(g, 0, 1, 5.0f, LINHA_B);
    trecho(g, 1, 2, 5.0f, LINHA_B);
    trecho(g, 2, 3, 8.0f, LINHA_B);
    trecho(g, 3, 4, 2.0f, LINHA_B);
    trecho(g, 4, 5, 9.0f, LINHA_B);
    trecho(g, 5, 6, 18.0f, LINHA_B);
    trecho(g, 6, 7, 5.0f, LINHA_B);
    trecho(g, 7, 8, 5.0f, LINHA_B);
    trecho(g, 8, 9, 2.0f, LINHA_B);
    trecho(g, 9, 10, 3.0f, LINHA_B);
    trecho(g, 10, 11, 3.0f, LINHA_B);

    // Onibus C: Juazeiro -> Av. Transnordestina -> CCA (volta por Petrolina)
    trecho(g, 0, 1, 5.0f, LINHA_C);
    trecho(g, 1, 16, 40.0f, LINHA_C);
    trecho(g, 16, 7, 5.0f, LINHA_C);
    trecho(g, 7, 10, 5.0f, LINHA_C);
    trecho(g, 10, 11, 3.0f, LINHA_C);
    trecho(g, 6, 12, 18.0f, LINHA_C);
    trecho(g, 12, 2, 7.0f, LINHA_C);
    trecho(g, 2, 15, 3.0f, LINHA_C);

    // Onibus D: Juazeiro -> Petrolina -> Av. 7 de Setembro -> Cohab -> CCA
    trecho(g, 0, 1, 5.0f, LINHA_D);
    trecho(g, 1, 3, 5.0f, LINHA_D);
    trecho(g, 3, 2, 5.0f, LINHA_D);
    trecho(g, 2, 12, 8.0f, LINHA_D);
    trecho(g, 12, 13, 7.0f, LINHA_D);
    trecho(g, 13, 14, 5.0f, LINHA_D);
    trecho(g, 14, 6, 5.0f, LINHA_D);
    trecho(g, 6, 7, 5.0f, LINHA_D);
    trecho(g, 7, 8, 5.0f, LINHA_D);
    trecho(g, 8, 9, 5.0f, LINHA_D);
    trecho(g, 9, 10, 3.0f, LINHA_D);
    trecho(g, 10, 11, 2.0f, LINHA_D);

    /* Velocidade usada na heuristica = maior velocidade do mapa (+1% de folga
       para erro de arredondamento). Assim h(n) nunca superestima o tempo real. */
    if (vel_max > 0.0f) {
        velocidade_media = vel_max * 1.01f;
    }
}
