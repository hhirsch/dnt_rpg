/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */

#include "fonte.h"
#include <stdio.h>

char arqFonte[255]; /* Actual used font */
fnt Fonte;          /* Active Font */
int Tamanho;        /* Size of Active Font */
int Alinhamento;    /* Align of Active Font */

/***********************************************************************
 *                             selFonte                                *
 ***********************************************************************/
int defineFont(const char* name, int align, int size)
{
   Tamanho = size;
   Alinhamento = align;
   if(strcmp(name,arqFonte)) 
   {
      FILE *arq;
      if ( !((arq) = fopen (name, "rb")))
      { 
         return(0);
      }
      
      fread(&Fonte,sizeof(fnt),1,arq);
      strcpy(arqFonte,name);
      fclose(arq);
      return(1);
   }
   return(1);
}

/***********************************************************************
 *                             escxy_Int                               *
 ***********************************************************************/
int write(SDL_Surface *screen,int x,int y,const char* text,int init,
          int end, int x1,int y1,int x2,int y2)
{
   if(text[0]=='\0') return(-1);
   int k,aux,ax,aux1,aux2; 
   int sobra;
   int c;

   if (Alinhamento == ALIGN_CENTER)
      x-=(strlen(text)*(Fonte.incCP) / 2);
   else if (Alinhamento == ALIGN_RIGHT)
      x-=(strlen(text)*(Fonte.incCP));

   k=y;
   for(aux=init;(aux<=end);aux++)
   {
     c=text[aux];
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
                 pixel_Set(screen,x,y);
                 if(Tamanho>1) pixel_Set(screen,x+1,y);
                 if(Tamanho>2) pixel_Set(screen,x+1,y+1);
                 if (Tamanho>3)
                 {
                    pixel_Set(screen,x,y+1);
                    pixel_Set(screen,x+2,y+1);
                    pixel_Set(screen,x+2,y+2);
                    pixel_Set(screen,x+1,y+2);
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
void write(SDL_Surface *screen,int x,int y,const char* text)
{
   write(screen,x,y,text,0,strlen(text)-1,0,0,screen->w-1,screen->h-1);
}

/***********************************************************************
 *                            escxy_Area                               *
 ***********************************************************************/
int write(SDL_Surface *screen,int x, int y,const char* text,int x1,int y1,
          int x2,int y2)
{
   return(write(screen,x,y,text,0,strlen(text)-1,x1,y1,x2,y2));
}

/***********************************************************************
 *                             escxy_Def                               *
 ***********************************************************************/
void write(SDL_Surface *screen,int x,int y,const char* text,
           int init,int end)
{
   write(screen,x,y,text,init,end,0,0,screen->w-1,screen->h-1);
}

/***********************************************************************
 *                           fonte_incCP                               *
 ***********************************************************************/
int font_incCP()
{
   return(Fonte.incCP+Tamanho-1);
}

/***********************************************************************
 *                           getTotalLines                             *
 ***********************************************************************/
int getTotalLines(string source)
{
   int i;
   char c = 0;
   int line = -1;
   for(i=0; (i < (int)source.length()) ; i++)
   {
      c = source.at(i);
      if(c == '|')
      {
         line++;
      }
   }
   /* If last character is different of |, theres a remanescent line */
   if(c != '|')
   {
      line++;
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

