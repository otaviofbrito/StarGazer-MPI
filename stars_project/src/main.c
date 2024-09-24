#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>

#include "image.h"
#include "read.h"

#define PGM_PATH "data/test.pgm"

int main(int argc, char *argv[])
{

  MPI_Init(&argc, &argv);
  int rank;
  int size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int total;
  image *img = NULL;

  if (rank == 0)
  {
    img = readPGM(PGM_PATH);
  }

  // cada processo possuira um bloco da imagem
  image *img_block = (image *)malloc(sizeof(image));
  int chunk_size = 0;

  MPI_Scatter(img->matrix, chunk_size, MPI_UNSIGNED_CHAR, img_block->matrix, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);

  printf("%d %d", img_block->height, img_block->width);

  /*
    Conta as estrelas
  */

  // int process_count = 0;

  // // Soma o resultado calculado em cada processo
  // MPI_Reduce(&process_count, &total, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  MPI_Finalize();
  return EXIT_SUCCESS;
}