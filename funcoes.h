
#ifndef UNTITLED1_BIBLIOTECA_H
#define UNTITLED1_BIBLIOTECA_H

int obter_indice(const char *tarefas_arquivo);

void adicionar_tarefas(const char *tarefas_arquivo);

void deletar_tarefas(int indice);

void listar_tarefas(int filtro_selecionado);

void editar_tarefa(int opcao_editar);

#endif //UNTITLED1_BIBLIOTECA_H
