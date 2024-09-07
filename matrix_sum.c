#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

void randomize_matrix(int *matrix, int rows, int cols)
{
  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
      matrix[i * cols + j] = rand() % 100;
  }
}

void sum_matrix(int *A, int *B, int *C, int dim)
{
  for (int i = 0; i < dim; i++)
    C[i] = A[i] + B[i];
}

void print_matrix(int *matrix, int rows, int cols)
{
  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      printf(" %d ", matrix[i * cols + j]);
    }
    printf("\n");
  }
}

int main(int argc, char *argv[])
{
  if (argc != 3)
  {
    printf("Usage: num_rows num_columns");
    return EXIT_FAILURE;
  }
  int rows = atoi(argv[1]);
  int cols = atoi(argv[2]);
  int dim = rows * cols;

  MPI_Init(&argc, &argv);
  int rank;
  int size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  srand(time(NULL) + rank);

  if (rows % size != 0)
  {
    if (rank == 0)
    {
      printf("Number of processes must be compatible with the number of rows!\n");
    }
    MPI_Finalize();
    return EXIT_FAILURE;
  }

  int *matrix_A = NULL;
  int *matrix_B = NULL;
  int *matrix_C = NULL;

  if (rank == 0)
  {
    matrix_A = (int *)malloc(dim * sizeof(int));
    matrix_B = (int *)malloc(dim * sizeof(int));
    matrix_C = (int *)malloc(dim * sizeof(int));
    randomize_matrix(matrix_A, rows, cols);
    randomize_matrix(matrix_B, rows, cols);
  }

  int chunk_size = (rows / size) * cols;
  int *portion_A = (int *)malloc(chunk_size * sizeof(int));
  int *portion_B = (int *)malloc(chunk_size * sizeof(int));

  MPI_Scatter(matrix_A, chunk_size, MPI_INT, portion_A, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Scatter(matrix_B, chunk_size, MPI_INT, portion_B, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);

  int *portion_C = (int *)malloc(chunk_size * sizeof(int));
  sum_matrix(portion_A, portion_B, portion_C, chunk_size);

  MPI_Gather(portion_C, chunk_size, MPI_INT, matrix_C, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);

  if (rank == 0)
  {
    printf("MATRIX A: \n");
    print_matrix(matrix_A, rows, cols);
    printf("\nMATRIX B: \n");
    print_matrix(matrix_B, rows, cols);
    printf("\nMATRIX C: \n");
    print_matrix(matrix_C, rows, cols);
    free(matrix_A);
    free(matrix_B);
    free(matrix_C);
  }

  free(portion_A);
  free(portion_B);
  free(portion_C);

  MPI_Finalize();
  return EXIT_SUCCESS;
}