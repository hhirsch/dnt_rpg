/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */

#include "fonte.h"
#include <stdio.h>

char* arqFonte=" "; //Arquivo de Fonte atualmente utilizado
fnt Fonte;          //Fonte Ativa
int Tamanho;        //Tamanho da Fonte Ativa 
int Alinhamento;    //Alinhamento da Fonte Ativa

int selFonte(char* nome, int alinha, int Tamanho)
{
   Tamanho = Tamanho;
   Alinhamento = alinha;
   if(strcmp(nome,arqFonte)) 
   {
      FILE *arq;
      if ( !((arq) = fopen (nome, "rb")))
      { 
         return(0);
      }
      
      fread(&Fonte,sizeof(fnt),1,arq);
      arqFonte = nome;
      fclose(arq);
      return(1);
   }
   return(1);
}

/* Procedimento interno e geral de escrita */
int escxy_Int(SDL_Surface *screen,int x,int y,const char* texto,int inic,
              int fim,
               int x1,int y1,int x2,int y2)
{
   if(texto[0]=='\0') return(-1);
   int k,aux,ax,aux1,aux2; 
   int sobra;
   int c;

   if (Alinhamento==CENTRALIZADO)
      x-=(strlen(texto)*(Fonte.incCP) / 2);
   else if (Alinhamento==DIREITA)
      x-=(strlen(texto)*(Fonte.incCP));

   k=y;
   for(aux=inic;(aux<=fim);aux++)
   {
     c=texto[aux];
     ax=x;
     for (aux1=0;aux1<16;aux1++)
     {
        sobra=Fonte.letra[c][aux1];
        for (aux2=0;aux2<16;aux2++)
        {
           if (sobra % 2==1)
           {
              pixel_Desenhar(screen,x,y,0);
              if(Tamanho>1) pixel_Desenhar(screen,x+1,y,0);
              if(Tamanho>2) pixel_Desenhar(screen,x+1,y+1,0);
              if (Tamanho>3)
              {
                 pixel_Desenhar(screen,x,y+1,0);
                 pixel_Desenhar(screen,x+2,y+1,0);
                 pixel_Desenhar(screen,x+2,y+2,0);
                 pixel_Desenhar(screen,x+1,y+2,0);
              }
           }
           sobra = sobra / 2;
           y++;
           if (Tamanho>2) y++;
        }
        y = k;
        x++;
     }

     if (Tamanho<2) x-=( x-ax-Fonte.incCP);
     else if (Tamanho==2) x-= x-ax-Fonte.incCP-1;
     else x-=(x-ax-Fonte.incCP) / 2;
     if(x+Fonte.incCP > x2)
     {
        x = x1;
        k += 11;
     }
     y = k;
   }
   return(k);
}


void escxy(SDL_Surface *screen,int x,int y,const char* texto)
{
   escxy_Int(screen,x,y,texto,0,strlen(texto)-1,0,0,screen->w-1,screen->h-1);
}

int escxy_Area(SDL_Surface *screen,int x, int y,const char* texto,int x1,int y1,
                int x2,int y2)
{
   return(escxy_Int(screen,x,y,texto,0,strlen(texto)-1,x1,y1,x2,y2));
}


void escxy_Def(SDL_Surface *screen,int x,int y,const char* texto,
               int inic,int fim)
{
   escxy_Int(screen,x,y,texto,inic,fim,0,0,screen->w-1,screen->h-1);
}

int fonte_incCP()
{
   return(Fonte.incCP+Tamanho-1);
}


