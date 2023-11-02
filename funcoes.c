#include "funcoes.h"
#include "stdio.h"
#include <stdlib.h>
#include <string.h>



// Obtem o numero de indice para criação das tarefas
int obter_indice() {
  FILE *arquivo_tarefas = fopen("tarefas.bin", "rb");
  if (arquivo_tarefas == NULL) {
    return 1;
  }

  struct tarefa tarefas;
  int maior_indice = 0;

  while (fread(&tarefas, sizeof(struct tarefa), 1, arquivo_tarefas) == 1) {
    if (tarefas.indice > maior_indice) {
      maior_indice = tarefas.indice;
    }
  }

  fclose(arquivo_tarefas);

  return maior_indice + 1;
}

// Limita a criação de 100 tarefas
int contar_tarefas() {
  FILE *arquivo_tarefas;
  struct tarefa temp_tarefas;
  int contador = 0;

  arquivo_tarefas = fopen("tarefas.bin", "rb");
  if (arquivo_tarefas == NULL) {
    printf("Erro ao abrir o arquivo.\n");
    return contador;
  }

  while (fread(&temp_tarefas, sizeof(temp_tarefas), 1, arquivo_tarefas)) {
    contador++;
  }

  fclose(arquivo_tarefas);

  return contador;
}

void menu() {

  int x;

  do {
    printf("-----------------------------------");
    printf("\n\nGerenciador de tarefas!\n\n");
    printf("1 - Criar tarefas\n");
    printf("2 - Deletar tarefa\n");
    printf("3 - Listar tarefas\n");
    printf("4 - Editar tarefas\n");
    printf("5 - Encerrar gerenciador\n");

    printf("\nEscolha uma opção: ");
    scanf("%d", &x);
    getchar();

    switch (x) {

    case 1: {
      adicionar_tarefas();
      break;
    }
    case 2: {
      deletar_tarefas();
      break;
    }
    case 3: {
      listar_tarefas();
      break;
    }
    case 5: {
      printf("\nFinalizando...\n");
      break;
    }
    default: {
      printf("\nOpcao invalida\n");
      break;
    }
    }

  } while (x != 5);
  return;
}

