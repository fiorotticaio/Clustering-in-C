#include "Ponto.h"
#include "Aresta.h"
#include "UnionTree.h"
#include "Grupo.h"

#include <time.h>

int main(int argc, char** argv) {
  
  clock_t start_total = clock();
  clock_t end_total;
  clock_t start = clock();
  clock_t end;


  start = clock();

  /**
   * @brief Significado de cada parâmetro:
   * argv[0] = ./trab1
   * argv[1] = nome_arquivo_entrada
   * argv[2] = k -> número de grupos
   * argv[3] = nome_arquivo_saida
   */

  if (argc < 4) {
    printf("Use: ./trab1 <nome_arquivo_entrada> k <nome_arquivo_saida>\n");
    return 1; 
  }
  
  /*======== Leitura dos dados de entrada ========*/
  Ponto** pontos = lePontos(argv[1]); // Vetor com todos os pontos

  end = clock();
  double time_spent_recursive = (double)(end - start) / CLOCKS_PER_SEC;
  printf("Tempo de execução da leitura do programa: %.6f segundos\n", time_spent_recursive);
  puts("");


  start = clock();

  /*======== Tratando os dados ========*/
  int qtdPontos = getQuantidadePontos(pontos); // Obtem quantidade de pontos

  // obtem o numero de arestas necessarias
  int qtdArestas = qtdPontos * (qtdPontos -1) / 2;

  /* Aresta: conexão entre dois pontos */
  Aresta** arestas = inicializaVetorArestas(qtdArestas);

  /* Preenche as arestas com as disntacias entre os pares de pontos unicos */
  preencheVetorArestas(arestas, qtdPontos, pontos);

  end = clock();
  time_spent_recursive = (double)(end - start) / CLOCKS_PER_SEC;
  printf("Tempo de execução do calculo das distâncias: %.6f segundos\n", time_spent_recursive);
  puts("");


  start = clock();

  qsort(arestas, qtdArestas, sizeof(Aresta*), compareArestas); // Ordena o vetor de arestas

  end = clock();
  time_spent_recursive = (double)(end - start) / CLOCKS_PER_SEC;
  printf("Tempo de execução da ordenação das distâncias: %.6f segundos\n", time_spent_recursive);
  puts("");


  start = clock();

  /*======== Aplicando o algoritmo ========*/
  int qtdGrupos = atoi(argv[2]);
  if (qtdGrupos > qtdPontos) {
    printf("O número de grupos não pode ser maior que a quantidade de pontos!\n");
    exit(1);
  }
  UnionTree* componentesConexas = inicializaUnionTree(qtdPontos);

  /* Utiliza o problema de union-find para conectar os pontos em grupos */
  conectaArestas(arestas, componentesConexas, qtdPontos, qtdArestas, qtdGrupos);

  end = clock();
  time_spent_recursive = (double)(end - start) / CLOCKS_PER_SEC;
  printf("Tempo de execução da obtenção da MST: %.6f segundos\n", time_spent_recursive);
  puts("");


  start = clock();

  /*======== Criando os grupos de pontos ========*/
  Grupo**  gruposPontos = inicializaVetorGrupos(qtdGrupos, qtdPontos, componentesConexas, pontos);
  preencheVetorGrupos(gruposPontos, qtdGrupos, componentesConexas, pontos, qtdPontos);
  ordenaTodosGrupos(gruposPontos, qtdGrupos);

  end = clock();
  time_spent_recursive = (double)(end - start) / CLOCKS_PER_SEC;
  printf("Tempo de execução da identificação dos grupos + ordenação: %.6f segundos\n", time_spent_recursive);
  puts("");


  start = clock();

  /*======== Imprimindo os grupos de pontos ========*/
  FILE* arquivoDeSaida = fopen(argv[3], "w");
  if (arquivoDeSaida == NULL) {
    printf("Erro ao abrir o arquivo %s\n", argv[3]);
    exit(1);
  }

  imprimeGrupos(gruposPontos, qtdGrupos, arquivoDeSaida);

  fclose(arquivoDeSaida);

  end = clock();
  time_spent_recursive = (double)(end - start) / CLOCKS_PER_SEC;
  printf("Tempo de execução da escrita do arquivo de saída: %.6f segundos\n", time_spent_recursive);
  puts("");

  /*======== Liberando toda a memória alocada ========*/
  destroiPontos(pontos);
  destroiVetorArestas(arestas, qtdArestas);
  destroiUnionTree(componentesConexas);
  destroiGrupos(gruposPontos, qtdGrupos);

  end_total = clock();
  time_spent_recursive = (double)(end_total - start_total) / CLOCKS_PER_SEC;
  printf("Tempo de execução do programa: %.6f segundos\n", time_spent_recursive);
  puts("");

  return 0;
}