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
int obter_indice(const char *tarefas_arquivo) {
  FILE *arquivo_tarefas = fopen("tarefas.bin", "rb");
  if (arquivo_tarefas == NULL) {
    return 1;
  }
  struct tarefa tarefas;
  int maior_indice = 0;

  // Lê o arquivo binário e atualiza o valor da varival para o maior indice
  while (fread(&tarefas, sizeof(struct tarefa), 1, arquivo_tarefas) == 1) {
    if (tarefas.indice > maior_indice) {
      maior_indice = tarefas.indice;
    }
  }

  fclose(arquivo_tarefas);

  // Retorna o maior indice + 1
  return maior_indice + 1;
}

// Adiciona uma nova tarefa à lista
void adicionar_tarefas(const char *tarefas_arquivo) {

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

  // variavel para contar as tarefas
  int contador = 0;
  //contador de tarefas
  while (fread(&tarefas, sizeof(tarefas), 1, arquivo_tarefas)) {
    contador++;
  }

  //verifica se o numero de tarefas foi atingido
  int numero_tarefas = contador;
  if (numero_tarefas >= 100) {
    printf("\nLimite de tarefas atingido.\n");
    return;
  }
  
   //escreve a categoria deseja criar
  printf("\nCategorias: \n");
  printf("1 - Educação\n2 - Saúde\n3 - Academia\n4 - Trabalho\n5 - Hobby\n");
  printf("\nEscolha a categoria: ");
  fgets(tarefas.categoria, sizeof(tarefas.categoria), stdin);
  tarefas.categoria[strcspn(tarefas.categoria, "\n")] = '\0';

  //condição para verificar a categoria escolhida
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

  //faz a descrição da tarefa
  printf("\nDigite a descricao da tarefa: ");
  fgets(tarefas.descricao, sizeof(tarefas.descricao), stdin);
  tarefas.descricao[strcspn(tarefas.descricao, "\n")] = '\0';

  //seleciona a prioridade da tarefa
  printf("\nDigite a prioridade da tarefa (1 - 10): ");
  scanf("%d", &tarefas.prioridade);
  getchar();

  if(tarefas.prioridade > 10 && tarefas.prioridade < 1){
    printf("\nPrioridade escolhida invalida\n");
    return;
  }

  //cria o status da tarefa 
  printf("\nStatus: \n");
  printf("1 - Em andamento\n2 - Concluida\n3 - Não iniciada\n");
  printf("\nEscolha o status: ");
  fgets(tarefas.status, sizeof(tarefas.status), stdin);
  tarefas.status[strcspn(tarefas.status, "\n")] = '\0';

  //condição para selecionar o status da tarefa
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

  //cria o indice para a tarefa
  tarefas.indice = obter_indice("tarefas.bin");
  
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
void deletar_tarefas(int indice) {
  FILE *arquivo_tarefas;
  struct tarefa tarefas;
  int encontrado = 0;

  // Abre o arquivo original para leitura e escrita em modo binário
  arquivo_tarefas = fopen("tarefas.bin", "rb+");
  if (arquivo_tarefas == NULL) {
    printf("Erro ao abrir o arquivo.\n");
    return;
  }

  // Procura pelo indice no arquivo original
  while (fread(&tarefas, sizeof(tarefas), 1, arquivo_tarefas)) {
    if (tarefas.indice == indice) {
      encontrado = 1;
      break;
    }
  }

  if (encontrado) {
    // Move o ponteiro do arquivo para a posição anterior a tarefa encontrado
    fseek(arquivo_tarefas, -sizeof(tarefas), SEEK_CUR);
    // Preenche o espaço da tarefa com zeros
    memset(&tarefas, 0, sizeof(tarefas));
    // Escreve os zeros no lugar da tarefa no arquivo
    fwrite(&tarefas, sizeof(tarefas), 1, arquivo_tarefas);
    printf("\nTarefa removida!\n");
  } else {
    printf("\nTarefa Inexistente\n");
  }

  fclose(arquivo_tarefas);
}

//Lista tarefas
void listar_tarefas(int filtro_selecionado) {
  FILE *arquivo_tarefas, *arquivo_filtros;
  struct tarefa tarefas;

  // Abre o arquivo binário para leitura
  arquivo_tarefas = fopen("tarefas.bin", "rb");
  if (arquivo_tarefas == NULL) {
    printf("Erro ao abrir o arquivo.\n");
    return;
  }

  // Abre o arquivo binário para leitura
  arquivo_filtros = fopen("filtros.txt", "w");
  if (arquivo_tarefas == NULL) {
    printf("Erro ao abrir o arquivo.\n");
    return;
  }


  // Lê todas as tarefas para um array
  struct tarefa todas_tarefas[100]; // Assumindo um número máximo de tarefas
  int num_tarefas = 0;

  while (fread(&tarefas, sizeof(struct tarefa), 1, arquivo_tarefas) == 1 && num_tarefas < 100) {
    todas_tarefas[num_tarefas] = tarefas;
    num_tarefas++;
  }

  // Ordena as tarefas
  switch (filtro_selecionado){

    
    // Filtra as tarefas por prioridade
    case 1:{  

      int prioridade_escolhida;

      printf("Digite a prioridade desejada (1 - 10): ");
      scanf("%d", &prioridade_escolhida);

      
      for (int i = 0; i < num_tarefas; i++){
        if (todas_tarefas[i].prioridade == prioridade_escolhida){
          printf("\n");
          printf("Prioridade: %d\n", todas_tarefas[i].prioridade);
          printf("Descrição: %s\n", todas_tarefas[i].descricao);
          printf("Categoria: %s\n", todas_tarefas[i].categoria);
          printf("Status: %s\n", todas_tarefas[i].status);
          printf("Indice: %d\n", todas_tarefas[i].indice);

          fprintf(arquivo_filtros, "Prioridade: %d | Categoria: %s | Status: %s | Descrição: %s\n" , todas_tarefas[i].prioridade, todas_tarefas[i].categoria, todas_tarefas[i].status, todas_tarefas[i].descricao);
          
        }
      }
      fclose(arquivo_filtros);
      break;
    }

    
    // Filtra as tarefas por categoria
    case 2:{ 
      
      char categoria_escolhida[11];
      int catego;

      for (int i = 0; i < num_tarefas - 1; i++) {
        for (int j = i + 1; j < num_tarefas; j++) {
          if (todas_tarefas[i].prioridade < todas_tarefas[j].prioridade) {
            struct tarefa temp = todas_tarefas[i];
            todas_tarefas[i] = todas_tarefas[j];
            todas_tarefas[j] = temp;
          }
        }
      }

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

      // Filtra e lista as tarefas que atendem aos critérios
      for (int i = 0; i < num_tarefas; i++) {
        if (strcmp(todas_tarefas[i].categoria, categoria_escolhida) == 0) {
          printf("\n");
          printf("Prioridade: %d\n", todas_tarefas[i].prioridade);
          printf("Descrição: %s\n", todas_tarefas[i].descricao);
          printf("Categoria: %s\n", todas_tarefas[i].categoria);
          printf("Status: %s\n", todas_tarefas[i].status);
          printf("Indice: %d\n", todas_tarefas[i].indice);


          fprintf(arquivo_filtros, "Prioridade: %d | Categoria: %s | Status: %s | Descrição: %s\n" , todas_tarefas[i].prioridade, todas_tarefas[i].categoria, todas_tarefas[i].status, todas_tarefas[i].descricao);
        }
      }
      fclose(arquivo_filtros);
      break;
      
    }

    
    // Ordenas as tarefas por status
    case 3:{

      char sta_escolhido[14];
      int stat;

      printf("\nStatus: \n");
      printf("1 - Em Andamento\n2 - Concluida\n3 - Não Iniciada\n");
      printf("\nEscolha o status da tarefa: ");
      scanf("%d", &stat);

      if (stat == 1) {
          strcpy(sta_escolhido, "Em Andamento");
      } else if (stat == 2) {
          strcpy(sta_escolhido, "Concluida");
      } else if (stat == 3) {
          strcpy(sta_escolhido, "Não Iniciada");
      } else {
          printf("Opção inválida.\n");
          break;
      }
      
      // Filtra e lista as tarefas que atendem aos critérios
      for (int i = 0; i < num_tarefas; i++) {
        if (strcmp(todas_tarefas[i].status, sta_escolhido) == 0) {
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
    case 4: {

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

          fprintf(arquivo_filtros, "Prioridade: %d | Categoria: %s | Status: %s | Descrição: %s\n" , todas_tarefas[i].prioridade, todas_tarefas[i].categoria, todas_tarefas[i].status, todas_tarefas[i].descricao);
        }
      }
      fclose(arquivo_filtros);
      break;
    }
    
    default:
      printf("Opção inválida.\n");
      break;
  }

  fclose(arquivo_tarefas);
}

void editar_tarefa(int opcao_editar){

  FILE *arquivo_tarefas;
  struct tarefa tarefas;

  arquivo_tarefas = fopen("tarefas.bin", "rb+");
  if (arquivo_tarefas ==NULL){
    printf("Erro ao abrir o arquivo\n");
    return;
  }

  //variaveis para identificação 
  int indce;
  

  switch (opcao_editar){

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