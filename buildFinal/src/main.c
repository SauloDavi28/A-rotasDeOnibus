#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include "../include/busca.h"
#include "../include/dados.h"
#include "../include/grafo.h"

void prompt_usuario(int*);
void ler_parada(const char*, int*);
void ler_opcao(const char*, int*);


int main(void){
	GRAFO* grafo = criar_grafo(NUM_PARADAS);
	if (grafo == NULL){
		fprintf(stderr,  "Erro ao criar grafo das rotas.\n");
		return 1;
	}
	carregar_malha(grafo);

	int continuar_programa = 1;
	while (continuar_programa){
		int escolha_ids[2];
		prompt_usuario(escolha_ids);

		printf("Paradas escolhidas: %d --> %d\n", escolha_ids[0], escolha_ids[1]);
		int pai[NUM_PARADAS];
		int linha_usada[NUM_PARADAS];
		float tempo_total;

		int resultado = busca_A_estrela(
			grafo,
			escolha_ids[0],
			escolha_ids[1],
			pai,
			linha_usada,
			&tempo_total
		);

		switch (resultado)
		{
		case 1:
			int caminho[NUM_PARADAS];
			int quantidade = 0;
			printf("Rota encontrada!\n");
			for(int parada = escolha_ids[1]; parada != -1 && quantidade < NUM_PARADAS; parada = pai[parada]){
				caminho[quantidade++] = parada;
			}

			for (int i = quantidade - 1; i >= 0; i--){
				int parada = caminho[i];
				printf("%s", nome_parada[parada]);

				if (i > 0){
					int proxima = caminho[i - 1];
					int linha = linha_usada[proxima];

					printf(" --[%s]--> ", nome_linha[linha]);
				}
			}
			printf("\n");
			printf("Tempo estimado: %0.2f minutos\n", tempo_total);

			break;
		case 0:
			printf("Rota inalcancavel...\n");
			break;
		default:
			printf("Erro! Parametros invalidos ou memoria insuficiente!\n");
			break;
		}

		ler_opcao(
			"Gostaria de testar outra rota? Sim(1) - Nao(0): ",
			&continuar_programa
		);
	}
	printf("Terminando programa...\n");
	liberar_grafo(grafo);
	return 0;
}

void prompt_usuario(int* escolha_ids){
	printf("\n\n");
	printf("Por favor consulte o fluxograma das paradas disponível na pasta docs.\n");
	printf("Em qual parada voce esta? Insira o indice correspondente.\n");
	printf("Univasf Campus Juazeiro - 0\n");
    printf("Terminal de Juazeiro - 1\n");           
    printf("Univasf Campus Petrolina - 2\n");       
    printf("Sementeira - 3\n");					
    printf("Petrape - 4\n");                        
    printf("Plante Bem - 5\n");                     
    printf("Feira da Cohab Massangano - 6\n");      
    printf("BR 407 - Izabela Construcoes - 7\n");   
    printf("Residencia Estudantil CCA - 8\n");      
    printf("CCA - Bloco Antigo - 9\n");             
    printf("CCA - Hospital Veterinario - 10\n");      
    printf("CCA - Bloco de Salas de Aula - 11\n");    
    printf("G Barbosa Petrolina - 12\n");             
    printf("Canteiro de Obras (Av. 7 Set.) - 13\n");  
    printf("Posto Sao Francisco (Av. 7 Set.) - 14\n");
    printf("Supermercado Regente - 15\n");           
    printf("Nutiva Agro (Transnordestina) - 16\n");

	ler_parada(
		"Em qual parada voce esta? Insira o indice correspondente: ",
		&escolha_ids[0]
	);
	ler_parada(
		"Qual o seu destino? Insira o indice correspondente: ",
		&escolha_ids[1]
	);
}

void ler_parada(const char *mensagem, int *parada) {
    char entrada[128];
    char *fim;
    long valor;

    do {
        printf("%s", mensagem);

        if (fgets(entrada, sizeof entrada, stdin) == NULL) {
            printf("Entrada encerrada (EOF). Terminando o programa.\n");
            *parada = -1;
            return;
        }

        errno = 0;
        valor = strtol(entrada, &fim, 10);

        if (errno == ERANGE ||
            fim == entrada ||
            (*fim != '\n' && *fim != '\0')) {
            printf("Erro! Digite somente um numero inteiro.\n");
            continue;
        }

        if (valor < 0 || valor >= NUM_PARADAS) {
            printf("Erro! Digite um indice valido entre 0 e 16.\n");
            continue;
        }

        *parada = (int)valor;
        return;

    } while (1);
}

void ler_opcao(const char *mensagem, int *opcao) {
    char entrada[128];
    char *fim;
    long valor;

    do {
        printf("%s", mensagem);

        if (fgets(entrada, sizeof entrada, stdin) == NULL) {
            printf("Entrada encerrada (EOF). Terminando o programa.\n");
            *opcao = 0;
            return;
        }

        errno = 0;
        valor = strtol(entrada, &fim, 10);

        if (errno == ERANGE ||
            fim == entrada ||
            (*fim != '\n' && *fim != '\0')) {
            printf("Erro! Digite 1 para continuar ou 0 para sair.\n");
            continue;
        }

        if (valor != 0 && valor != 1) {
            printf("Erro! Digite 1 para continuar ou 0 para sair.\n");
            continue;
        }

        *opcao = (int)valor;
        return;

    } while (1);
}
