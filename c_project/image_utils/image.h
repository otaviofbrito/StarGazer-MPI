#pragma once

#define AUTHOR "# https://github.com/otaviofbrito/StarGazer-MPI"
#define COLS 20

typedef struct
{
  int width, height, maxValue;
  int *matrix;
} image;

image *newImage(int width, int height, int maxValue);
void freeImage(image *img);
image *readPGM(const char *file);
void savePGM(image *img, char *name);