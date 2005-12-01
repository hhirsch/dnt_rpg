#include "texture.h"
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/********************************************************************
 *                      Cria Lista de Texturas                      *
 ********************************************************************/
texturasLista::texturasLista()
{
   primeiro = (texture*) malloc(sizeof(texture));
   primeiro->proximo = primeiro;
   primeiro->anterior = primeiro;
   numTexturas = 0;
}

/********************************************************************
 *                      Destroi a Lista de Texturas                 *
 ********************************************************************/
texturasLista::~texturasLista()
{
   texture* tex = primeiro->proximo;
   texture* proximo;
   while(numTexturas)
   {
      proximo = tex->proximo;
      free(tex);
      numTexturas--; 
      tex = proximo;
   }
   delete(primeiro);

}

/*********************************************************************
 *                   Retorna o nome da textura                       *
 *********************************************************************/
char* texturasLista::nomeTextura(GLuint ID)
{
   int aux=0;
   texture* tex = primeiro->proximo;
   while(aux < numTexturas)
   {
      if(tex->indice == ID)
      {
         return(tex->nome);
      }
      tex = tex->proximo;
      aux++;
   }
   return(NULL);
}

/********************************************************************
 *                      Retorna o indice da Textura                 *
 ********************************************************************/
int texturasLista::idTextura(char* nome, GLuint* R, GLuint* G, GLuint* B)
{
   /* procura pela textura */
   int aux=0;
   texture* tex = primeiro->proximo;
   while(aux < numTexturas)
   {
      if(!strcmp(tex->nome, nome))
      {
         *R = tex->R; *G = tex->G; *B = tex->B;
         return(tex->indice); //a textura ja esta presente 
      }
      tex = tex->proximo;
      aux++;
   }
   return(-1);
}

/********************************************************************
 *                      Insere Textura na Lista                     *
 ********************************************************************/
void texturasLista::inserirTextura(char* arqNome, char*nome, 
                                   GLuint R, GLuint G, GLuint B)
{

   /* procura pela textura */
   int aux=0;
   texture* tex = primeiro->proximo;
   while(aux < numTexturas)
   {
      if(!strcmp(tex->nome, nome))
      {
         tex->numReferencias++;
         if(tex->R != R) 
         {
            tex->R = R;
            tex->G = G;
            tex->B = B;
         }
         return; //a textura ja esta presente 
      }
      tex = tex->proximo;
      aux++;
   }

   SDL_Surface* img = IMG_Load(arqNome);
   if(!img)
   {
      printf("Error Opening Texture: %s\n",arqNome);
      return;
   }

   /* Insere realmente a textura */ 
   tex = (texture*) malloc(sizeof(texture));
   tex->proximo = primeiro->proximo;
   tex->anterior = primeiro;
   tex->proximo->anterior = tex;
   tex->anterior->proximo = tex;

   tex->nome = (char*) malloc((strlen(nome)+1)*sizeof(char));
   tex->arqNome = (char*) malloc((strlen(arqNome)+1)*sizeof(char));

   strcpy(tex->arqNome,arqNome);
   strcpy(tex->nome,nome);

   SDL_Surface *imgPotencia = SDL_CreateRGBSurface(SDL_HWSURFACE,
                       img->w,img->h,32,
                       0x000000FF,0x0000FF00,0x00FF0000,0xFF000000);
   
   SDL_BlitSurface(img,NULL,imgPotencia,NULL);
 
   tex->R = R;
   tex->G = G;
   tex->B = B;

   glGenTextures(1, &(tex->indice));
   glBindTexture(GL_TEXTURE_2D, tex->indice);
   glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,imgPotencia->w,imgPotencia->h, 
                0, GL_RGBA, GL_UNSIGNED_BYTE, imgPotencia->pixels);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

   numTexturas++;
   tex->numReferencias = 1;

   /* Libera a memoria utilizada */
   SDL_FreeSurface(img);
   SDL_FreeSurface(imgPotencia);
}

/********************************************************************
 *                      Retira Textura da Lista                     *
 ********************************************************************/
void texturasLista::retirarTextura(char* nome)
{
   int aux=0;
   texture* tex = primeiro->proximo;
   while(aux < numTexturas)
   {
      if(!strcmp(tex->nome, nome))
      {
         tex->numReferencias--;
         if(tex->numReferencias == 0)
         {
            tex->anterior->proximo = tex->proximo;
            tex->proximo->anterior = tex->anterior;
            numTexturas--;
            free(tex);
         }
         return;
      }
      tex = tex->proximo;
      aux++;
   }
}

/********************************************************************
 *                      Retira Textura da Lista                     *
 ********************************************************************/
void texturasLista::retirarTextura(GLuint ID)
{
   int aux=0;
   texture* tex = primeiro->proximo;
   while(aux < numTexturas)
   {
      if(tex->indice == ID)
      {
         tex->numReferencias--;
         if(tex->numReferencias == 0)
         {
            tex->anterior->proximo = tex->proximo;
            tex->proximo->anterior = tex->anterior;
            numTexturas--;
            free(tex);
         }
         return;
      }
      tex = tex->proximo;
      aux++;
   }
}



