#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "image.h"

#define min(a, b) ((a) < (b) ? (a) : (b))

void distance_transform(image *img)
{
  int max = -1;
  int rows = img->height;
  int columns = img->width;

  for (int i = 1; i < rows; i++)
  {
    for (int j = 1; j < columns; j++)
    {
      int p = img->matrix[i * columns + j];
      if (p != 0)
      {
        int a = img->matrix[(i - 1) * columns + j];
        int b = img->matrix[i * columns + (j - 1)];
        img->matrix[i * columns + j] = min(a + 1, b + 1);
      }
    }
  }

  for (int i = rows - 2; i >= 0; i--)
  {
    for (int j = columns - 2; j >= 0; j--)
    {
      int p = img->matrix[i * columns + j];
      if (p != 0)
      {
        int a = img->matrix[(i + 1) * columns + j];
        int b = img->matrix[i * columns + (j + 1)];
        int current = img->matrix[i * columns + j];
        img->matrix[i * columns + j] = min(min(a + 1, b + 1), current);

        if (img->matrix[i * columns + j] > max)
        {
          max = img->matrix[i * columns + j];
        }
      }
    }
  }
  img->maxValue = max;
}

void normalize(image *img, int scale)
{
  int level = img->maxValue;
  int *t = (int *)malloc((scale + 1) * sizeof(int));
  if (!t)
  {
    fprintf(stderr, "Memory allocation failed\n");
    exit(EXIT_FAILURE);
  }

  // Create the normalization table `t`
  for (int i = 0; i <= scale; i++)
  {
    int x = level - i;
    t[i] = (int)((float)x / level * scale);
    printf("%d ", t[i]);
  }

  // Normalize the image matrix values
  for (int i = 0; i < img->height; i++)
  {
    for (int j = 0; j < img->width; j++)
    {
      int pixel_value = img->matrix[i * img->width + j];

      // Ensure the pixel value is within the range [0, scale]
      if (pixel_value < 0)
      {
        pixel_value = 0; // Optionally, handle negative values
      }
      else if (pixel_value > scale)
      {
        pixel_value = scale; // Clamp to the max value
      }

      // Assign the normalized value from the lookup table `t`
      img->matrix[i * img->width + j] = t[pixel_value];
    }
  }

  free(t);
}
