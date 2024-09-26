#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "image.h"

int _find(int parent[], int i)
{
  while (parent[i] != i)
    i = parent[i];
  return i;
}

void _union(int parent[], int i, int j)
{
  int x = find(parent, i);
  int y = find(parent, j);
  parent[y] = x;
}

void label(image *img)
{
  int rows = img->height;
  int cols = img->width;

  int numLabel = 0;
  int parent[1000];
  for (int i = 0; i < 1000; i++)
  {
    parent[i] = i;
  }
  for (int i = 1; i < rows; i++)
  {
    for (int j = 1; j < cols; j++)
    {
      int p = img->matrix[i * cols + j];
      int r = img->matrix[(i - 1) * cols + j];
      int t = img->matrix[i * cols + j - 1];
      if (p != 0)
      {
        if (r == 0 && t == 0)
          img->matrix[i * cols + j] = ++numLabel;
        if (r != 0 && t == 0)
          img->matrix[i * cols + j] = r;
        if (r == 0 && t != 0)
          img->matrix[i * cols + j] = t;
        if (r != 0 && t != 0 && t == r)
          img->matrix[i * cols + j] = r;
        if (r != 0 && t != 0 && t != r)
        {
          img->matrix[i * cols + j] = t;
          _union(parent, r, t);
        }
      }
    }
  }
  for (int i = 0; i < rows * cols; i++)
  {
    img->matrix[i] = _find(parent, img->matrix[i]);
  }
  img->maxValue = numLabel;
}