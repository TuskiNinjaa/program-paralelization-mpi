// ----------------------------------------------------------------------------
// histograma_seq: Calcula o histograma de um vetor de n inteiros positivos,
//                 em m intervalos de valores (SEQUENCIAL)
// Supor que: m <= máximo do vetor - mínimo do vetor + 1
// Compilar com:
// 	gcc histograma_seq.c -o histograma_seq -Wall
// Executar por linha de comando:
// 	./histograma_seq arquivo_entrada arquivo_saida

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

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
void escreveArqOut(char* nomeArqOut, int m, int min, int max, int l, int *h)
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
		int limInf = min + i*l;
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

	for (int i = 1 ; i < n ; i++)
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
	// Inicialização -----------------------------------------------------------

	if(argc != 3)
	{
		printf("O programa foi executado com argumentos incorretos.\n") ;
		printf("Uso: ./histograma_seq arquivo_entrada arquivo_saida\n") ;
		exit(1) ;
	}

	int n, // Número de elementos do vetor de entrada
		 m; // Número de intervalos do histograma
	FILE *arqIn = leNArqIn(argv[1], &n, &m); // Lê n e m do arquivo de entrada

	// Aloca vetores para dados de entrada e saída
	int *v = (int *) malloc(n * sizeof(int)); // Vetor de entrada com n elementos
	int *h = (int *) malloc(m * sizeof(int)); // Histograma de saída com m elementos

	if ((v == NULL) || (h == NULL))
	{
		printf("\nErro na alocação de estruturas\n") ;
		exit(1) ;
	}

	// Lê vetor de entrada do arquivo de entrada
	leVetInArqIn(arqIn, n, v);

	struct timeval tIni, tFim; // Obtém instante de tempo inicial
	gettimeofday(&tIni, 0);

	// Corpo principal ---------------------------------------------------------

	// Calcula valores mínimo e máximo do vetor de entrada
	int min, max;
	minimo_maximo(n, &min, &max, v);

	// Calcula largura da faixa de valores de cada intervalo do histograma
	int l = (max - min + 1) / m;

	// Calcula histograma
	histograma(n, m, min, l, v, h);

	// Finalização -------------------------------------------------------------

	gettimeofday(&tFim, 0); // Obtém instante de tempo final

	// Calcula tempo de execução em segundos
	long segundos = tFim.tv_sec - tIni.tv_sec;
	long microsegundos = tFim.tv_usec - tIni.tv_usec;
	double tempo = segundos + (microsegundos * 1e-6);
	printf("Tempo=%.2f segundos\n", tempo);

	// Escreve histograma no arquivo texto de saída
	escreveArqOut(argv[2], m, min, max, l, h);

	// Libera vetores de entrada e saída
	free(v);
	free(h);

	return 0;
}
// ----------------------------------------------------------------------------
