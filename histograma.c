// ----------------------------------------------------------------------------
// Nome dos alunos do grupo:
// Vitor Yuske Watanabe - 2020.1905.058-4
// Raissa Rinaldi Yoshioka - 2020.1905.049-5
// ----------------------------------------------------------------------------
// histograma:
// 	Calcula o histograma de um vetor de n inteiros positivos, em m intervalos de valores (PARALELO)
// 	Supor que: m <= máximo do vetor - mínimo do vetor + 1
// Compilar com:
// 	mpicc histograma.c -o histograma -Wall
// Executar por linha de comando:
//	mpirun -use-hwthread-cpus histograma entrada.txt saida.txt
//	mpirun -np 4 histograma entrada.txt saida.txt

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define RAIZ 0

// ----------------------------------------------------------------------------
FILE *leNArqIn(char *nomeArqIn, int *n, int *m)
{
	FILE *arqIn = fopen(nomeArqIn, "rt"); // Abre arquivo texto de entrada

	if (arqIn == NULL)
	{
		printf("\nArquivo texto de entrada não encontrado\n");
		exit(1);
	}

	fscanf(arqIn, "%d", n); // Lê tamanho do vetor de entrada do arquivo de entrada
	fscanf(arqIn, "%d", m); // Lê número de intervalos do arquivo de entrada

	return arqIn;
}

// ----------------------------------------------------------------------------
void leVetInArqIn(FILE *arqIn, int n, int *v)
{
	// Lê vetor de entrada do arquivo de entrada
	for (int i = 0; i < n; i++)
		fscanf(arqIn, "%d", &(v[i]));

	fclose(arqIn); // Fecha arquivo de entrada
}

// ----------------------------------------------------------------------------
void escreveArqOut(char *nomeArqOut, int m, int min, int max, int l, int *h)
{
	FILE *arqOut = fopen(nomeArqOut, "wt"); // Cria arquivo texto de saída

	if (arqOut == NULL)
	{
		printf("\nArquivo texto de saída não pode ser criado\n");
		exit(1);
	}

	// Escreve histograma no arquivo de saída
	for (int i = 0; i < m; i++)
	{
		int limInf = min + i * l;
		int limSup = limInf + l - 1;
		if (i == m - 1)
			limSup = max;
		fprintf(arqOut, "%d %d %d\n", limInf, limSup, h[i]);
	}

	fclose(arqOut); // Fecha arquivo de saída
}

// ----------------------------------------------------------------------------
void minimo_maximo(int n, int *min, int *max, int *v)
{
	*min = v[0];
	*max = v[0];

	for (int i = 1; i < n; i++)
		if (v[i] < *min)
			*min = v[i];
		else if (v[i] > *max)
			*max = v[i];
}

// ----------------------------------------------------------------------------
void histograma(int n, int m, int min, int l, int *v, int *h)
{
	// Inicializa histograma com 0
	for (int i = 0; i < m; i++)
		h[i] = 0;

	// Para cada elemento do vetor de entrada, determina a que intervalo do histograma ele pertence
	for (int i = 0; i < n; i++)
	{
		int j = (v[i] - min) / l;
		if (j >= m)
			j = m - 1;

		h[j]++;
	}
}

// ----------------------------------------------------------------------------
int main(int argc, char **argv)
{
	int idProcesso,
		nProcessos,
		minLocal,
		maxLocal,
		l;

	MPI_Init(&argc, &argv); // Inicializa ambiente de execução do MPI

	MPI_Comm_rank(MPI_COMM_WORLD, &idProcesso); // Obtém id do processo
	MPI_Comm_size(MPI_COMM_WORLD, &nProcessos); // Obtém número de processos

	if (idProcesso == RAIZ)
	{
		// Inicialização -----------------------------------------------------------

		int n, // Número de elementos do vetor de entrada
			m, // Número de intervalos do histograma
			min,
			max;

		if (argc != 3)
		{
			printf("O programa foi executado com argumentos incorretos.\n");
			printf("Uso: ./histograma_seq arquivo_entrada arquivo_saida\n");
			exit(1);
		}

		FILE *arqIn = leNArqIn(argv[1], &n, &m); // Lê n e m do arquivo de entrada

		// Aloca vetores para dados de entrada e saída
		int *v = (int *)malloc(n * sizeof(int));				// Vetor de entrada com n elementos
		int *h = (int *)malloc(m * sizeof(int));				// Histograma de saída com m elementos
		int *tamanho = (int *)malloc(nProcessos * sizeof(int)); // Vetor de tamanhos dos subvetores usados por cada processo
		int *posicao = (int *)malloc(nProcessos * sizeof(int)); // Posição inicial da parte do vetor de entrada (e depois de saída) de cada processo

		if ((v == NULL) || (h == NULL) || (tamanho == NULL) || (posicao == NULL))
		{
			printf("\nErro na alocação de estruturas\n");
			MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
		}

		// Lê vetor de entrada do arquivo de entrada
		leVetInArqIn(arqIn, n, v);

		// Inicializa a contagem o tempo
		double tini, tfim;
		tini = MPI_Wtime();

		// Corpo principal ---------------------------------------------------------

		// Calcula os valores armazenados nos vetores tamanho e posicao
		int divisao = n / nProcessos;
		int resto = n % nProcessos;
		int aux = 0;

		for (int i = 0; i < nProcessos; i++)
		{
			if (i < resto)
				tamanho[i] = divisao + 1;
			else
				tamanho[i] = divisao;
			posicao[i] = aux;
			aux += tamanho[i];
		}

		// Envia o tamanho dos subvetores de cada processo e mPart
		MPI_Scatter(tamanho, 1, MPI_INT, MPI_IN_PLACE, 0, MPI_INT, RAIZ, MPI_COMM_WORLD);
		MPI_Bcast(&m, 1, MPI_INT, RAIZ, MPI_COMM_WORLD);

		// Envia parte do vetor de entrada para cada processo
		MPI_Scatterv(v, tamanho, posicao, MPI_INT, MPI_IN_PLACE, 0, MPI_INT, RAIZ, MPI_COMM_WORLD);

		// Atualiza o valor do n para o subvetor acessado pelo processo RAIZ
		n = tamanho[0];

		// Histograma auxiliar com mPart elementos
		int *hPart = (int *)malloc(m * sizeof(int));

		if (hPart == NULL)
		{
			printf("\nErro na alocação de estruturas\n");
			MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
		}

		// Calcula valores mínimo e máximo do vetor de entrada
		minimo_maximo(n, &minLocal, &maxLocal, v);

		MPI_Reduce(&minLocal, &min, 1, MPI_INT, MPI_MIN, RAIZ, MPI_COMM_WORLD);
		MPI_Reduce(&maxLocal, &max, 1, MPI_INT, MPI_MAX, RAIZ, MPI_COMM_WORLD);

		// Calcula largura da faixa de valores de cada intervalo do histograma
		l = (max - min + 1) / m;
		MPI_Bcast(&l, 1, MPI_INT, RAIZ, MPI_COMM_WORLD);

		// Calcula histograma
		histograma(n, m, minLocal, l, v, hPart);

		MPI_Reduce(hPart, h, m, MPI_INT, MPI_SUM, RAIZ, MPI_COMM_WORLD);

		// Finalização -------------------------------------------------------------

		// Encerra a contagem do tempo e imprime a medição realizada
		tfim = MPI_Wtime();
		printf("Tempo=%.2f segundos\n", tfim - tini);

		// Escreve histograma no arquivo texto de saída
		escreveArqOut(argv[2], m, min, max, l, h);

		// Libera vetores de entrada e saída
		free(v);
		free(h);
		free(hPart);
		free(tamanho);
		free(posicao);
	}
	else
	{
		int nPart, mPart;
		// Recebe número de elementos da parte do vetor de entrada desse processo
		MPI_Scatter(NULL, 1, MPI_INT, &nPart, 1, MPI_INT, RAIZ, MPI_COMM_WORLD);
		MPI_Bcast(&mPart, 1, MPI_INT, RAIZ, MPI_COMM_WORLD);

		// Aloca vetores para parte dos dados de entrada e saída do processo
		int *vPart = (int *)malloc(nPart * sizeof(int));
		int *hPart = (int *)malloc(mPart * sizeof(int));

		if ((vPart == NULL) || (hPart == NULL))
		{
			printf("\nErro na alocação de estruturas\n");
			MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
		}

		// Recebe parte do vetor de entrada desse processo
		MPI_Scatterv(NULL, NULL, NULL, MPI_INT, vPart, nPart, MPI_INT, RAIZ, MPI_COMM_WORLD);

		// Calcula valores mínimo e máximo do vetor de entrada
		minimo_maximo(nPart, &minLocal, &maxLocal, vPart);

		MPI_Reduce(&minLocal, NULL, 1, MPI_INT, MPI_MIN, RAIZ, MPI_COMM_WORLD);
		MPI_Reduce(&maxLocal, NULL, 1, MPI_INT, MPI_MAX, RAIZ, MPI_COMM_WORLD);

		MPI_Bcast(&l, 1, MPI_INT, RAIZ, MPI_COMM_WORLD);

		// Calcula histograma
		histograma(nPart, mPart, minLocal, l, vPart, hPart);

		MPI_Reduce(hPart, NULL, mPart, MPI_INT, MPI_SUM, RAIZ, MPI_COMM_WORLD);

		// Libera vetores para parte dos dados de entrada e saída do processo
		free(vPart);
		free(hPart);
	}

	MPI_Finalize(); // Finaliza ambiente de execução do MPI

	return 0;
}
// ----------------------------------------------------------------------------
