#include <stdio.h>
#include "funcoes.h"

int main(void) {
  

  int x;
  int filtro_selecionado, indice;

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
        adicionar_tarefas("tarefas.bin");
        break;
      }
      case 2: {

        printf("\nDeletar tarefas: \n");

        printf("\nDigite o indice da tarefa a ser deletada: ");
        scanf("%d", &indice);
        
        
        deletar_tarefas(indice);
        break;
      }
      case 3: {
  
        printf("\nLista de Tarefas:\n\n");
  
        printf("Escolha o filtro: \n\n");
        printf("1 - Prioridade\n2 - Categoria\n3 - Status\n4 - Prioridade e Categoria\n");
        printf("\nFiltro escolhido: ");
        scanf("%d", &filtro_selecionado);
        printf("\n");
          
        listar_tarefas(filtro_selecionado);
        break;
      }
      case 4:{

        int opcao_editar;

        printf("\nEditar tarefas: \n\n");

        printf("1 - Prioridade\n");
        printf("2 - Categoria\n");
        printf("3 - Status\n");
        printf("4 - Descrição\n");

        //opção para escolher oque editar
        printf("\nO que deseja editar: ");
        scanf("%d", &opcao_editar);

        
        editar_tarefa(opcao_editar);
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

}
