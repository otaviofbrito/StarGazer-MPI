#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "image.h"

void threshold_image(image *img, int threshold)
{

   if (!img)
   {
      printf("Erro: imagem não inicializada");
   }

   for (int i = 0; i < img->height * img->width; i++)
   {
      if (img->matrix[i] >= threshold)
      {
         img->matrix[i] = 1;
         // image -> matrix[i]
      }
      else
      {
         img->matrix[i] = 0;
         // image -> matrix[i]
      }
   }

   img->maxValue = 1;
}