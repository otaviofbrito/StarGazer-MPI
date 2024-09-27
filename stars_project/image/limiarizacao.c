#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "image.h"
#include "limiarizacao.h"

void threshold_image(image *img, int threshold) {

   // image* image = newImage(img->width, img->height, img->maxValue); 

   if(!img) 
   {
     printf("Erro: imagem não inicializada");
   }

    for (int i = 0; i < img->height * img->width; i++) 
    {
       if(img->matrix[i] >= threshold) {
        img->matrix[i] = 1; 
        // image -> matrix[i]
       } else {
        img->matrix[i] = 0; 
        // image -> matrix[i]
       } 
    }

    // salvar em uma nova imagem limiarizada? 
} 