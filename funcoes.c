#include "funcoes.h"
#include "stdio.h"
#include <stdlib.h>
#include <string.h>

struct tarefa {
  int prioridade;
  char categoria[11];
  char descricao[100];
  char status[13];
  int indice;
  int max_caracteres[20];
};

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

// Apaga uma tarefa da lista
void deletar_tarefas() {
  FILE *arquivo_tarefas;
  struct tarefa tarefas;
  int indice_para_deletar;
  int encontrado = 0;

  // Abre o arquivo original para leitura e escrita em modo binário
  arquivo_tarefas = fopen("tarefas.bin", "rb+");
  if (arquivo_tarefas == NULL) {
    printf("Erro ao abrir o arquivo.\n");
    return;
  }

  printf("\nDeletar tarefas: \n");

  printf("\nDigite o indice da tarefa a ser deletada: ");
  scanf("%d", &indice_para_deletar);

  // Procura pela descrição no arquivo original
  while (fread(&tarefas, sizeof(tarefas), 1, arquivo_tarefas)) {
    if (tarefas.indice == indice_para_deletar) {
      encontrado = 1;
      break;
    }
  }

  if (encontrado) {
    // Move o ponteiro do arquivo para a posição anterior ao cliente encontrado
    fseek(arquivo_tarefas, -sizeof(tarefas), SEEK_CUR);
    // Preenche o espaço do cliente com zeros
    memset(&tarefas, 0, sizeof(tarefas));
    // Escreve os zeros no lugar do cliente no arquivo
    fwrite(&tarefas, sizeof(tarefas), 1, arquivo_tarefas);
    printf("\nTarefa removida!\n");
  } else {
    printf("\nTarefa Inexistente\n");
  }

  fclose(arquivo_tarefas);
}

void listar_tarefas() {
  FILE *arquivo_tarefas;
  struct tarefa tarefas;

  int c;

  // Abre o arquivo binário para leitura
  arquivo_tarefas = fopen("tarefas.bin", "rb");
  if (arquivo_tarefas == NULL) {
    printf("Erro ao abrir o arquivo.\n");
    return;
  }

  printf("\nLista de Tarefas:\n\n");

  printf("Escolha o filtro: \n\n");
  printf("1 - Prioridade\n2 - Categoria\n3 - Status\n4 - Prioridade e Categoria\n");
  printf("\nFiltro escolhido: ");
  scanf("%d", &c);
  printf("\n");

  // Lê todas as tarefas para um array
  struct tarefa todas_tarefas[100]; // Assumindo um número máximo de tarefas (ajuste conforme necessário)
  int num_tarefas = 0;

  while (fread(&tarefas, sizeof(struct tarefa), 1, arquivo_tarefas) == 1 && num_tarefas < 100) {
    todas_tarefas[num_tarefas] = tarefas;
    num_tarefas++;
  }

  // Ordena as tarefas
  switch (c) {

    
    // Ordena as tarefas por prioridade
    case 1:{ 
      for (int i = 0; i < num_tarefas - 1; i++) {
        for (int j = i + 1; j < num_tarefas; j++) {
          if (todas_tarefas[i].prioridade < todas_tarefas[j].prioridade) {
            struct tarefa temp = todas_tarefas[i];
            todas_tarefas[i] = todas_tarefas[j];
            todas_tarefas[j] = temp;
          }
        }
      }
      // Lista as tarefas ordenadas
      for (int i = 0; i < num_tarefas; i++) {
        if (todas_tarefas[i].prioridade > 0) {
          printf("\n");
          printf("Prioridade: %d\n", todas_tarefas[i].prioridade);
          printf("Descrição: %s\n", todas_tarefas[i].descricao);
          printf("Categoria: %s\n", todas_tarefas[i].categoria);
          printf("Status: %s\n", todas_tarefas[i].status);
          printf("Indice: %d\n", todas_tarefas[i].indice);
        }
      }
      break;
    }

    
    // Ordena as tarefas por categoria
    case 2:{ 
      for (int i = 0; i < num_tarefas - 1; i++) {
        for (int j = i + 1; j < num_tarefas; j++) {
          if (strcmp(todas_tarefas[i].categoria, todas_tarefas[j].categoria) > 0) {
            struct tarefa temp = todas_tarefas[i];
            todas_tarefas[i] = todas_tarefas[j];
            todas_tarefas[j] = temp;
          }
        }
      }
      // Lista as tarefas ordenadas
      for (int i = 0; i < num_tarefas; i++) {
        if (todas_tarefas[i].prioridade > 0) {
          printf("\n");
          printf("Prioridade: %d\n", todas_tarefas[i].prioridade);
          printf("Descrição: %s\n", todas_tarefas[i].descricao);
          printf("Categoria: %s\n", todas_tarefas[i].categoria);
          printf("Status: %s\n", todas_tarefas[i].status);
          printf("Indice: %d\n", todas_tarefas[i].indice);
        }
      }
      break;
    }

    
    // Ordenas as tarefas por status
    case 3:{
      for (int i = 0; i < num_tarefas - 1; i++) {
        for (int j = i + 1; j < num_tarefas; j++) {
          if (strcmp(todas_tarefas[i].status, todas_tarefas[j].status) > 0) {
            struct tarefa temp = todas_tarefas[i];
            todas_tarefas[i] = todas_tarefas[j];
            todas_tarefas[j] = temp;
          }
        }
      }
      // Lista as tarefas ordenadas
      for (int i = 0; i < num_tarefas; i++) {
        if (todas_tarefas[i].prioridade > 0) {
          printf("\n");
          printf("Prioridade: %d\n", todas_tarefas[i].prioridade);
          printf("Descrição: %s\n", todas_tarefas[i].descricao);
          printf("Categoria: %s\n", todas_tarefas[i].categoria);
          printf("Status: %s\n", todas_tarefas[i].status);
          printf("Indice: %d\n", todas_tarefas[i].indice);
        }
      }
      break;
    }
    // Ordena as tarefas por prioridade e categoria
    case 4:{
      
      //variaveis de categoria e prioridade
      char categoria_escolhida[11];
      int prioridade_escolhida, catego;

      printf("\nCategorias: \n");
      printf("1 - Educação\n2 - Saúde\n3 - Academia\n4 - Trabalho\n5 - Hobby\n");
      printf("\nEscolha a categoria: ");
      scanf("%d", &catego);

      //condição para escolha da categoria
      if (catego == 1){
        strcpy(categoria_escolhida, "Educação");
      } else if (catego == 2){
        strcpy(categoria_escolhida, "Saúde");
      } else if (catego == 3){
        strcpy(categoria_escolhida, "Academia");
      } else if (catego == 4){
        strcpy(categoria_escolhida, "Trabalho");
      } else if (catego == 5){
        strcpy(categoria_escolhida, "Hobby");
      } else {
        printf("Opção inválida.\n");
        break;
      }

      printf("Digite a prioridade desejada: ");
      scanf("%d", &prioridade_escolhida);

      // Filtra e lista as tarefas que atendem aos critérios
      for (int i = 0; i < num_tarefas; i++) {
        if (todas_tarefas[i].prioridade == prioridade_escolhida &&
            strcmp(todas_tarefas[i].categoria, categoria_escolhida) == 0) {
          printf("\n");
          printf("Prioridade: %d\n", todas_tarefas[i].prioridade);
          printf("Descrição: %s\n", todas_tarefas[i].descricao);
          printf("Categoria: %s\n", todas_tarefas[i].categoria);
          printf("Status: %s\n", todas_tarefas[i].status);
          printf("Indice: %d\n", todas_tarefas[i].indice);
        }
      }
      break;
    }
    
    default:
      printf("Opção inválida.\n");
      break;
  }
  
  fclose(arquivo_tarefas);
}

void editar_tarefa(){

  FILE *arquivo_tarefas;
  struct tarefa tarefas;

  arquivo_tarefas = fopen("tarefas.bin", "rb+");
  if (arquivo_tarefas ==NULL){
    printf("Erro ao abrir o arquivo\n");
    return;
  }

  //variaveis para identificação 
  int a;
  int indce;
  
  printf("\nEditar tarefas: \n\n");

  printf("1 - Prioridade\n");
  printf("2 - Categoria\n");
  printf("3 - Status\n");
  printf("4 - Descrição\n");

  //opção para escolher oque editar
  printf("\nO que deseja editar: ");
  scanf("%d", &a);

  switch (a){

    //edita a prioridade da tarefa
    case 1:{
       printf("\nDigite o indice da tarefa: ");
      scanf("%d", &indce);

      //procura a tarefa pelo indice 
      while (fread(&tarefas, sizeof(tarefas), 1, arquivo_tarefas)) {
        if (tarefas.indice == indce){

          //variavel que armazena a nova prioridade
          int prio;
  
          printf("Digite a nova prioridade: ");
          scanf("%d", &prio);

          //altera a prioridade da tarefa
          tarefas.prioridade = prio;

          //salva no arquivo a alteração
          fseek(arquivo_tarefas, -sizeof(struct tarefa), SEEK_CUR);
          fwrite(&tarefas, sizeof(struct tarefa), 1, arquivo_tarefas);

          //fecha o arquivo
          fclose(arquivo_tarefas);
            
          printf("\nPrioridade alterada para: %d\n", tarefas.prioridade);
          return;
        }  
      } 
    }

    //edita a categoria
    case 2:{
      printf("\nDigite o indice da tarefa: ");
      scanf("%d", &indce);

      //procura a tarefa pelo indice
      while (fread(&tarefas, sizeof(tarefas), 1, arquivo_tarefas)) {
        if (tarefas.indice == indce){

          //varivel que armazena a nova categoria
          int cate;
  
          printf("\nCategorias: \n");
          printf("\n1 - Educação\n2 - Saúde\n3 - Academia\n4 - Trabalho\n5 - Hobby\n");
          printf("\nDigite a nova categoria: ");
          scanf("%d", &cate);

          //condições que armazenam a nova categoria
          if (cate == 1){
            strcpy(tarefas.categoria, "Educação");
          } else if (cate == 2){
            strcpy(tarefas.categoria, "Saúde");
          } else if (cate == 3){
          strcpy(tarefas.categoria, "Academia");
          } else if (cate == 4){
            strcpy(tarefas.categoria, "Trabalho");
          } else if (cate == 5){
            strcpy(tarefas.categoria, "Hobby");
          } else {
            printf("\nOpção invalida\n");
            return;
          }

          //faz alteração no arquivo
          fseek(arquivo_tarefas, -sizeof(struct tarefa), SEEK_CUR);
          fwrite(&tarefas, sizeof(struct tarefa), 1, arquivo_tarefas);

          //fecha o arquivo
          fclose(arquivo_tarefas);
  
          printf("\nCategoria alterada para: %s\n", tarefas.categoria);
          return;
        }   
      }
    }

    //edita o status da tarefa
    case 3:{
      printf("\nDigite o indice da tarefa: ");
      scanf("%d", &indce);

      //procura a tarefa pelo indice
      while (fread(&tarefas, sizeof(tarefas), 1, arquivo_tarefas)) {
        if (tarefas.indice == indce){

          //variavel que armazena o novo status
          int sts;

          printf("\nStatus: \n");
          printf("\n1 - Em Andamento\n2 - Concluida\n3 - Não iniciada\n");
          printf("\nDigite a nova categoria: ");
          scanf("%d", &sts);
          
           //condições que verifica o novo status
          if (sts == 1){
            strcpy(tarefas.status, "Em Andamento");
          } else if (sts == 2){
            strcpy(tarefas.status, "Concluida");
          } else if (sts == 3){
          strcpy(tarefas.status, "Não iniciada");
          } else {
            printf("\nOpção invalida\n");
            return;
          }

          //Salva as alterações no arquivo
          fseek(arquivo_tarefas, -sizeof(struct tarefa), SEEK_CUR);
          fwrite(&tarefas, sizeof(struct tarefa), 1, arquivo_tarefas);

          //fecha o arquivo
          fclose(arquivo_tarefas);

          printf("\nStatus alterado para: %s\n", tarefas.status);
          return;
        }  
      } 
    }

    //edita a descrição
    case 4:{

        printf("\nDigite o indice da tarefa: ");
      scanf("%d", &indce);

      //verifica a tarefa pelo indice
      while (fread(&tarefas, sizeof(tarefas), 1, arquivo_tarefas)) {
        if (tarefas.indice == indce){

          //variavel que armazena a nova descrição
          char nova_descricao[100];

          printf("\nDigite a nova descrição: ");
          scanf("%s", &nova_descricao);

          //faz a alteração da descrição
          strcpy(tarefas.descricao, nova_descricao);

          //salva no arquivo a nova descrição
          fseek(arquivo_tarefas, -sizeof(struct tarefa), SEEK_CUR);
          fwrite(&tarefas, sizeof(struct tarefa), 1, arquivo_tarefas);

          //fecha o arquivo
          fclose(arquivo_tarefas);

          //imprime a alteração para confirmar o funcionamento
          printf("\nDescrição alterada para: %s\n", tarefas.descricao);
          return;
        }  
      }  
    } 

    //caso selecione uma opção incorreta
    default:{
      printf("\nOpção inválida.\n");
      break;
    }
  }
}