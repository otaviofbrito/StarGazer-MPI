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
    perror("[ERROR] Error opening file while reading image");
    exit(1);
  }

  fgets(lines, 80, fp);
  if (lines[0] != 'P' || lines[1] != '2')
  {
    fprintf(stderr, "Invalid format: %s\n", lines);
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

  printf("H: %d, W: %d\n", largura, altura);
  printf("Max gray level: %d\n", maxValor);

  return img;
}

void savePGM(image *img, char *name)
{
  FILE *fp;
  char tp = '2';
  fp = fopen(name, "w");
  if (fp == NULL)
  {
    perror("[ERROR] Error opening file while saving image");
    exit(1);
  }
  fprintf(fp, "P%c\n", tp);
  fprintf(fp, "%d  %d\n", img->width, img->height);
  fprintf(fp, "%d\n", img->maxValue);
  int c = 0;
  for (int i = 0; i < img->height * img->width; i++)
  {
    fprintf(fp, "%3d", img->matrix[i]);
    c++;
    if (c > COLS)
    {
      fprintf(fp, "\n");
      c = 0;
    }
  }
  fclose(fp);
}