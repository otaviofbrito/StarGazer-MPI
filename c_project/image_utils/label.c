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
  int x = _find(parent, i);
  int y = _find(parent, j);
  parent[y] = x;
}

void label(image *img)
{
  int rows = img->height;
  int cols = img->width;

  int numLabel = 0;
  int max_labels = rows * cols;
  int *parent = (int *)malloc(max_labels * sizeof(int));

  if (parent == NULL)
  {
    fprintf(stderr, "Erro ao alocar memória!\n");
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < max_labels; i++)
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

  free(parent);
}

int count_labels(image *img)
{
  int rows = img->height;
  int cols = img->width;

  int *visited = (int *)calloc(img->maxValue + 1, sizeof(int));
  if (visited == NULL)
  {
    fprintf(stderr, "Erro ao alocar memória!\n");
    exit(EXIT_FAILURE);
  }
  int count = 0;

  for (int i = 0; i < rows * cols; i++)
  {
    int label = img->matrix[i];

    if (label != 0 && visited[label] == 0)
    {
      visited[label] = 1;
      count++;
    }
  }

  free(visited);
  return count;
}

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
        if (i == h)
          break;
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
        if (i == v)
          break;
      }
    }
  }

  return dups;
}