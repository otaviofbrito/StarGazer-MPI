#pragma once

typedef struct
{
  int width;
  int height;
  int maxValue;
  unsigned char **matrix;
} image;

image *newImage(int width, int height, int maxValue);

void freeImage(image *img);