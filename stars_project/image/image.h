#pragma once

typedef struct
{
  int width, height, maxValue;
  int *matrix;
} image;

image *newImage(int width, int height, int maxValue);

void freeImage(image *img);

image *readPGM(const char *file);