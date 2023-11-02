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

// Adiciona uma nova tarefa à lista
void adicionar_tarefas() {

  FILE *arquivo_tarefas;
  struct tarefa tarefas;

  // Abre o arquivo binário para leitura e escrita
  arquivo_tarefas = fopen("tarefas.bin", "rb+");
  if (arquivo_tarefas == NULL) {
    // Se o arquivo não existir, tenta criar
    arquivo_tarefas = fopen("tarefas.bin", "wb+");
    if (arquivo_tarefas == NULL) {
      printf("Erro ao abrir/criar o arquivo.\n");
      return;
    }
  }

  // Verificar o número de tarefas ja esta no maximo
  int numero_tarefas = contar_tarefas();
  if (numero_tarefas >= 100) {
    printf("\nLimite de tarefas atingido (100 tarefasno máximo).\n");
    return;
  }

  printf("\nCategorias: \n");
  printf("1 - Educação\n2 - Saúde\n3 - Academia\n4 - Trabalho\n5 - Hobby\n");
  printf("\nEscolha a categoria: ");
  fgets(tarefas.categoria, sizeof(tarefas.categoria), stdin);
  tarefas.categoria[strcspn(tarefas.categoria, "\n")] = '\0';


  if (strcmp(tarefas.categoria, "1") == 0) {
    strcpy(tarefas.categoria, "Educação");
  } else if (strcmp(tarefas.categoria, "2") == 0) {
    strcpy(tarefas.categoria, "Saúde");
  } else if (strcmp(tarefas.categoria, "3") == 0) {
    strcpy(tarefas.categoria, "Academia");
  } else if (strcmp(tarefas.categoria, "4") == 0) {
    strcpy(tarefas.categoria, "Trabalho");
  } else if (strcmp(tarefas.categoria, "5") == 0) {
    strcpy(tarefas.categoria, "Hobby");
  } else {
    printf("\nOpção inválida\n");
    return;
  }
  while ((getchar()) != '\n');


  printf("\nDigite a descricao da tarefa: ");
  fgets(tarefas.descricao, sizeof(tarefas.descricao), stdin);
  tarefas.descricao[strcspn(tarefas.descricao, "\n")] = '\0';
  
  printf("\nDigite a prioridade da tarefa (0-10): ");
  scanf("%d", &tarefas.prioridade);
  getchar();

  printf("\nStatus: \n");
  printf("1 - Em andamento\n2 - Concluida\n3 - Não iniciada\n");
  printf("\nEscolha o status: ");
  fgets(tarefas.status, sizeof(tarefas.status), stdin);
  tarefas.status[strcspn(tarefas.status, "\n")] = '\0';

  if (strcmp(tarefas.status, "1") == 0) {
    strcpy(tarefas.status, "Em Andamento");
  } else if (strcmp(tarefas.status, "2") == 0) {
    strcpy(tarefas.status, "Concluida");
  } else if (strcmp(tarefas.status, "3") == 0) {
    strcpy(tarefas.status, "Não iniciada");
  } else {
    printf("\nOpção inválida\n");
    return;
  }
  
  tarefas.indice = obter_indice();
  
  // Abre o arquivo binário para escrita em modo de adição
  arquivo_tarefas = fopen("tarefas.bin", "ab");
  if (arquivo_tarefas == NULL) {
    printf("Erro ao abrir o arquivo.\n");
    return;
  }

  fwrite(&tarefas, sizeof(struct tarefa), 1, arquivo_tarefas);

  fclose(arquivo_tarefas);

  printf("\nNovo tarefa criada com sucesso!\n");
}

