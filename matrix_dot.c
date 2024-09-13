#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define MAX_VALUE 1000.0

void print_matrix(double *matrix, int rows, int cols)
{
  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      printf(" %f ", matrix[i * cols + j]);
    }
    printf("\n");
  }
}

void random_fill_matrix(double *matrix, int rows, int cols)
{
  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      matrix[i * cols + j] = ((float)rand() / (float)RAND_MAX) * MAX_VALUE;
    }
  }
}

void dot_product(double *A, double *B, double *C, int colsA, int colsB)
{
  for (int j = 0; j < colsB; j++)
  {
    C[j] = 0;
    for (int k = 0; k < colsA; k++)
    {
      C[j] += A[k] * B[k * colsB + j];
    }
  }
}

int main(int argc, char *argv[])
{

  if (argc != 2)
  {
    printf("Usage: %s num_colsA_rowsB\n", argv[0]);
    return EXIT_FAILURE;
  }

  MPI_Init(&argc, &argv);
  int colsA, rowsB;
  colsA = rowsB = atoi(argv[1]);

  int rank;
  int size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int rowsA = size;
  int colsB = size;

  srand(time(NULL) + rank);

  double *matrix_A = NULL;
  double *matrix_B = (double *)malloc((rowsB * colsB) * sizeof(double));
  double *matrix_C = NULL;
  if (rank == 0)
  {
    matrix_A = (double *)malloc((rowsA * colsA) * sizeof(double));
    matrix_C = (double *)malloc((rowsA * colsB) * sizeof(double));
    random_fill_matrix(matrix_A, rowsA, colsA);
    random_fill_matrix(matrix_B, rowsB, colsB);
  }

  int chunk_size_a = colsA;
  int chunk_size_c = colsB;
  double *local_A = (double *)malloc(chunk_size_a * sizeof(double));
  double *local_C = (double *)malloc(chunk_size_c * sizeof(double));

  // Distribui as linhas de matrix_A
  MPI_Scatter(matrix_A, chunk_size_a, MPI_DOUBLE, local_A, chunk_size_a, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  // Envia matrix_B para todos os processos
  MPI_Bcast(matrix_B, (rowsB * colsB), MPI_DOUBLE, 0, MPI_COMM_WORLD);

  // Calcula o produto escalar de uma linha de A com B
  dot_product(local_A, matrix_B, local_C, colsA, colsB);

  // Reúne os resultados de todos os processos
  MPI_Gather(local_C, chunk_size_c, MPI_DOUBLE, matrix_C, chunk_size_c, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  if (rank == 0)
  {
    printf("\nMATRIX A = \n");
    print_matrix(matrix_A, size, colsA);
    printf("\nMATRIX B = \n");
    print_matrix(matrix_B, rowsB, size);
    printf("\nMATRIX C (AxB) = \n");
    print_matrix(matrix_C, rowsA, colsB);
    free(matrix_A);
    free(matrix_C);
  }

  free(matrix_B);
  free(local_A);
  free(local_C);

  MPI_Finalize();
  return EXIT_SUCCESS;
}