#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "image.h"

image *readPGM(const char *file)
{
  FILE *fp;
  char formato[3];
  int altura, largura, maxValor;


  fp = fopen(file, "rb");
  if (fp == NULL)
  {
    perror("Erro ao abrir o arquivo");
    exit(1);
  }

  fscanf(fp, "%s", formato);

  if (formato[0] != 'P' || formato[1] != '2')
  {
    fprintf(stderr, "Formato inválido: %s\n", formato);
    fclose(fp);
    exit(1);
  }

  // Skip comments # TODO

  fscanf(fp, "%d %d", &largura, &altura);
  fscanf(fp, "%d", &maxValor);
  fgetc(fp);

  image *img = newImage(largura, altura, maxValor);

  for (int i = 0; i < altura; i++)
  {
    fread(img->matrix[i], sizeof(unsigned char), largura, fp);
  }

  fclose(fp);

  printf("Formato: %s\n", formato);
  printf("Largura: %d, Altura: %d\n", largura, altura);
  printf("Valor máximo: %d\n", maxValor);

  return img;
}