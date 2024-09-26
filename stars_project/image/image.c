#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "image.h"
image *newImage(int width, int height, int maxValue)
{
  image *imagePGM = (image *)malloc(sizeof(image));

  imagePGM->width = width;
  imagePGM->height = height;
  imagePGM->maxValue = maxValue;

  imagePGM->matrix = malloc(height * width * sizeof(int));

  return imagePGM;
}

void freeImage(image *img)
{
  free(img->matrix);
  free(img);
}

image *readPGM(const char *file)
{
  FILE *fp;
  char lines[100];
  int altura, largura, maxValor;

  fp = fopen(file, "rb");
  if (fp == NULL)
  {
    perror("Erro ao abrir o arquivo");
    exit(1);
  }

  fgets(lines, 80, fp);
  if (lines[0] != 'P' || lines[1] != '2')
  {
    fprintf(stderr, "Formato inválido: %s\n", lines);
    fclose(fp);
    exit(1);
  }

  fgets(lines, 80, fp);
  while (strchr(lines, '#'))
    fgets(lines, 80, fp);

  sscanf(lines, "%d %d", &largura, &altura);
  fscanf(fp, "%d", &maxValor);
  fgetc(fp);

  image *img = newImage(largura, altura, maxValor);

  for (int i = 0; i < altura * largura; i++)
  {
    int k;
    fscanf(fp, "%d", &k);
    img->matrix[i] = k;
  }

  fclose(fp);

  printf("Largura: %d, Altura: %d\n", largura, altura);
  printf("Valor máximo: %d\n", maxValor);

  return img;
}