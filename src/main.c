#include <stdio.h>
#include <stdlib.h>
#include "../include/busca.h"
#include "../include/dados.h"
#include "../include/grafo.h"

void prompt_usuario(int*);


int main(){
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
		printf("Gostaria de testar outra rota? Sim(1) - Nao(0): ");
		scanf("%d", &continuar_programa);
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

	scanf("%d", &escolha_ids[0]);
	while(escolha_ids[0] < 0 || escolha_ids[0] >= NUM_PARADAS){
		printf("Erro! escolha um indice valido! (0 a 16): ");
		scanf("%d", &escolha_ids[0]);
	}

	printf("Qual o seu destino?\n");
	scanf("%d", &escolha_ids[1]);
	while(escolha_ids[1] < 0 || escolha_ids[1] >= NUM_PARADAS){
		printf("Erro! escolha um indice valido! (0 a 16): ");
		scanf("%d", &escolha_ids[1]);
	}
}
