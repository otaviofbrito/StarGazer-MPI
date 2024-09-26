#include <stdlib.h>

#include "image.h"
image *newImage(int width, int height, int maxValue)
{
  image *imagePGM = (image *)malloc(sizeof(image));

  imagePGM->width = width;
  imagePGM->height = height;
  imagePGM->maxValue = maxValue;

  imagePGM->matrix = (unsigned char *)malloc(height * width * sizeof(unsigned char));

  return imagePGM;
}

void freeImage(image *img)
{
  free(img->matrix);
  free(img);
}