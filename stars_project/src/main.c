#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>

#include "image.h"
#include "read.h"

#define PGM_PATH "data/test.pgm"

int main(int argc, char *argv[])
{
  int rank;
  int size;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int total;
  int N;

  image *img = NULL;

  if (rank == 0)
  {
    img = readPGM(PGM_PATH);
  }

  // Compartilha a dimensao da matiz com os outros processos;
  MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);

  // cada processo possuira um bloco da imagem
  image *img_block = (image *)malloc(sizeof(image));
  int block_size = img_block->width * img_block->height;

  // Distribuir os blocos entre os processos
  if (rank == 0)
  {
  }

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