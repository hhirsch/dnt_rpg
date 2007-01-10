/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */

#include "fonte.h"
#include <stdio.h>

char arqFonte[255]; //Arquivo de Fonte atualmente utilizado
fnt Fonte;          //Fonte Ativa
int Tamanho;        //Tamanho da Fonte Ativa 
int Alinhamento;    //Alinhamento da Fonte Ativa

/***********************************************************************
 *                             selFonte                                *
 ***********************************************************************/
int selFonte(const char* nome, int alinha, int Tamanho)
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
      strcpy(arqFonte,nome);
      fclose(arq);
      return(1);
   }
   return(1);
}

/***********************************************************************
 *                             escxy_Int                               *
 ***********************************************************************/
int escxy_Int(SDL_Surface *screen,int x,int y,const char* texto,int inic,
              int fim, int x1,int y1,int x2,int y2)
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
     while(c < 0) /* Corrige leitura de caracteres nao padrao */
     {
        c = 256 + c;       
     }
     ax=x;
     
     if(c != '|')
     {
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
        if (Tamanho<2) x = (ax+Fonte.incCP);
        else if (Tamanho==2) x = (ax+Fonte.incCP)+1;
        else x = (ax+Fonte.incCP) / 2;
        if( (x > x2) )
        {
           x = x1;
           k += 11;
        }

     }
     else
     {
        x = x1;
        k += 11;
     }
     y = k;
   }
   return(k);
}

/***********************************************************************
 *                               escxy                                 *
 ***********************************************************************/
void escxy(SDL_Surface *screen,int x,int y,const char* texto)
{
   escxy_Int(screen,x,y,texto,0,strlen(texto)-1,0,0,screen->w-1,screen->h-1);
}

/***********************************************************************
 *                            escxy_Area                               *
 ***********************************************************************/
int escxy_Area(SDL_Surface *screen,int x, int y,const char* texto,int x1,int y1,
                int x2,int y2)
{
   return(escxy_Int(screen,x,y,texto,0,strlen(texto)-1,x1,y1,x2,y2));
}

/***********************************************************************
 *                             escxy_Def                               *
 ***********************************************************************/
void escxy_Def(SDL_Surface *screen,int x,int y,const char* texto,
               int inic,int fim)
{
   escxy_Int(screen,x,y,texto,inic,fim,0,0,screen->w-1,screen->h-1);
}

/***********************************************************************
 *                           fonte_incCP                               *
 ***********************************************************************/
int fonte_incCP()
{
   return(Fonte.incCP+Tamanho-1);
}

/***********************************************************************
 *                           getTotalLines                             *
 ***********************************************************************/
int getTotalLines(string source)
{
   Uint32 i;
   char c;
   int line = 0;
   for(i=0; (i < source.length()) ; i++)
   {
      c = source.at(i);
      if(c == '|')
      {
         line++;
      }
   }
   return(line);
}

/***********************************************************************
 *                             copyLines                               *
 ***********************************************************************/
string copyLines(string source, int firstLine, int lastLine)
{
   string result = "";
   char c;
   Uint32 i;
   int line = 0;
   /* Positionate the string to the first desired line */
   for(i=0; ( (i < source.length()) && (line < firstLine)) ; i++)
   {
      c = source.at(i);
      if(c == '|')
      {
         line++;
      }
   }
   
   /* Copy to the desired line (or end of the string, witch occurs first) */
   for(; ( (i<source.length()) && (line <= lastLine)); i++)
   {
      c = source.at(i);
      result += c;
      if(c == '|')
      {
         line++;
      }
   }

   return(result);
}

