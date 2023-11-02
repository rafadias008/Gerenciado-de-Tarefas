#include "funcoes.h"
#include "stdio.h"
#include <stdlib.h>
#include <string.h>

struct tarefa {
  char prioridade[3];
  char categoria[10];
  char descricao[100];
  char status[13];
  int indice;
  int max_caracteres[20];
};

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
