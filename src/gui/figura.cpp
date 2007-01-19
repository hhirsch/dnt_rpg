/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */


#include "figura.h"
#include <stdio.h>
#include <SDL/SDL_image.h>


figura::~figura()
{
   if (fig)
      SDL_FreeSurface(fig);
}

figura::figura(int x,int y,int w,int h,const char* arquivo)
{
   x1 = x;
   y1 = y;
   x2 = x+w;
   y2 = y+h; 
 
   if(arquivo!=NULL)
   {
      SDL_Surface* img = IMG_Load(arquivo);
      if(!img)
      {
         printf("Can't Open Image file: %s\n DNT should crash soon.\n",arquivo);
         return;
      }
      //fig = img;
      fig = SDL_CreateRGBSurface(SDL_HWSURFACE,
                       img->w,img->h,32,
                       0x000000FF,0x0000FF00,0x00FF0000,0xFF000000);
      SDL_BlitSurface(img,NULL,fig,NULL);
      SDL_FreeSurface(img);
      if ( fig == NULL )
      {
         erro_Mensagem("Não foi possível carregar figura\n",10);
      }
      else
      {
         x2 = x1 + fig->w;
         y2 = y1 + fig->h;
      }
   }
   else if( (w > 0) && (h > 0) )
   {
      fig = SDL_CreateRGBSurface(SDL_HWSURFACE,
                       w,h,32,
                       0x000000FF,0x0000FF00,0x00FF0000,0xFF000000);
   }
   else
   {
      fig = NULL;
   }
}

void figura::Desenhar(int Xjan,int Yjan,int salvar,SDL_Surface *screen)
{
   SDL_Rect Ret;
   Ret.x = x1+Xjan;
   Ret.y = y1+Yjan;
   Ret.w = fig->w;
   Ret.h = fig->h;
   SDL_BlitSurface(fig,NULL,screen,&Ret);
   if(salvar)
   {
      //SDL_Flip(screen);
      //SDL_GL_SwapBuffers();
      //AtualizaTela2D(screen);
   }
}
