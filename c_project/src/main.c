#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "image.h"
#include "label.h"
#include "threshold.h"
#include "distance_transform.h"

// Encontra estrelas duplicadas na divisao central de cada bloco
int find_duplicates(image *img)
{
  int dups = 0;
  int v = img->height / 2; // Linha central vertical
  int h = img->width / 2;  // Linha central horizontal

  for (int i = 0; i < img->width; i++)
  {
    if (img->matrix[v * img->width + i] != 0)
    {
      dups++;
      while (i < img->width && img->matrix[v * img->width + i] != 0)
      {
        i++;
      }
    }
  }

  for (int i = 0; i < img->height; i++)
  {
    if (img->matrix[i * img->width + h] != 0)
    {
      dups++;
      while (i < img->height && img->matrix[i * img->width + h] != 0)
      {
        i++;
      }
    }
  }

  return dups;
}

int main(int argc, char *argv[])
{
  if (argc != 3)
  {
    printf("Usage: %s <threshold> <path/to/image.pgm>\n", argv[0]);
    return EXIT_FAILURE;
  }

  const int THRESHOLD = atoi(argv[1]);
  const char *PGM_PATH = argv[2];

  int rank;
  int size;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  // Calcula quantos blocos por linha/coluna
  int sqrt_p = sqrt(size); // Número de blocos por linha/coluna
  if (sqrt_p * sqrt_p != size)
  {
    if (rank == 0)
    {
      printf("O número de processos precisa ser um quadrado perfeito!\n");
    }
    MPI_Finalize();
    return EXIT_FAILURE;
  }
  // soma total e dimensao da matriz
  int total;
  int N, M;

  image *img = NULL;

  if (rank == 0)
  {
    img = readPGM(PGM_PATH);
    N = img->width;
    M = img->height;
  }

  // Compartilha a dimensao da matriz com os outros processos;
  MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&M, 1, MPI_INT, 0, MPI_COMM_WORLD);

  if (N % sqrt_p != 0 || M % sqrt_p != 0)
  {
    if (rank == 0)
    {
      printf("As dimensões da imagem não são divisíveis igualmente pelo número de processos.\n");
    }
    MPI_Finalize();
    return EXIT_FAILURE;
  }

  // Calcula o tamanho dos blocos
  // cada processo possuíra um bloco da imagem.

  int block_size_x = N / sqrt_p;
  int block_size_y = M / sqrt_p;

  image *img_block = newImage(block_size_x, block_size_y, 0);
  image *buffer = NULL;

  // Distribuir os blocos entre os processos
  if (rank == 0)
  {
    // separar primeiro bloco para o processo raiz
    for (int i = 0; i < block_size_y; i++)
    {
      for (int j = 0; j < block_size_x; j++)
      {
        img_block->matrix[i * block_size_x + j] = img->matrix[i * N + j];
      }
    }

    buffer = newImage(block_size_x, block_size_y, 0);

    // Enviar os outros blocos para os demais processos
    for (int p = 1; p < size; p++)
    {
      int start_row = (p / sqrt_p) * block_size_y;
      int start_col = (p % sqrt_p) * block_size_x;

      int k = 0, l = 0;
      for (int i = start_row; i < start_row + block_size_y; i++, k++)
      {
        l = 0;
        for (int j = start_col; j < start_col + block_size_x; j++, l++)
        {
          buffer->matrix[k * block_size_x + l] = img->matrix[i * N + j];
        }
      }

      // Send block
      MPI_Send(buffer->matrix, block_size_x * block_size_y, MPI_INT, p, 0, MPI_COMM_WORLD);
    }
    freeImage(buffer);
    // freeImage(img);
  }
  else
  {
    // Se nao for processo raiz -> Receber o bloco
    MPI_Recv(img_block->matrix, block_size_x * block_size_y, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  }

  printf("Processo %d recebeu bloco de %dx%d\n", rank, img_block->width, img_block->height);

  /*
   * Operacoes em cada bloco
   * 1 Limiarizacao > Tansformada de distancia >
   * 2 limiarizacao>  rotulacao > contagem
   */
  threshold_image(img_block, THRESHOLD);
  distance_transform(img_block);
  threshold_image(img_block, 3);
  label(img_block);
  int process_count = count_labels(img_block);

  printf("Processo %d encontrou %d estrelas.\n", rank, process_count);

  // Soma o resultado calculado em cada processo
  MPI_Reduce(&process_count, &total, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  // Checa por estrelas duplicadas na imagem original
  int dups = 0;
  if (rank == 0)
  {
    threshold_image(img, THRESHOLD);
    dups = find_duplicates(img);
    printf("\nEstrelas duplicadas: %d\n", dups);
    savePGM(img, "data/output.pgm");
    free(img);
  }

  if (rank == 0)
  {
    printf("\nTotal de estrelas encontradas: %d\n", total - dups);
  }

  freeImage(img_block);
  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Finalize();
  return EXIT_SUCCESS;
}