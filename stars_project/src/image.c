#include <stdlib.h>

#include "image.h"
image *newImage(int width, int height, int maxValue)
{
  image *imagePGM = (image *)malloc(sizeof(image));

  imagePGM->width = width;
  imagePGM->height = height;
  imagePGM->maxValue = maxValue;

  imagePGM->matrix = (unsigned char **)malloc(height * sizeof(unsigned char *));

  for (int i = 0; i < height; i++)
  {
    imagePGM->matrix[i] = (unsigned char *)malloc(width * sizeof(unsigned char));
  }

  return imagePGM;
}

void freeImage(image *img)
{
  for (int i = 0; i < img->height; i++)
  {
    free(img->matrix[i]);
  }
  free(img->matrix);
  free(img);
}