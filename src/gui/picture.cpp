/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */


#include "picture.h"
#include <stdio.h>
#include <SDL/SDL_image.h>

/******************************************************
 *                    Destructor                      *
 ******************************************************/
picture::~picture()
{
   if (fig)
      SDL_FreeSurface(fig);
}

/******************************************************
 *                     Constructor                    *
 ******************************************************/
picture::picture(int x,int y,int w,int h,const char* arquivo)
{
   x1 = x;
   y1 = y;
   x2 = x+w;
   y2 = y+h;
   type = GUI_PICTURE;
 
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
         printf("Warn: Can't Load Picture, may crash soon (out of memory)!\n");
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

/******************************************************
 *                        draw                        *
 ******************************************************/
void picture::draw(SDL_Surface *screen)
{
   if(fig == NULL)
   {
      return;
   }
   SDL_Rect Ret;
   Ret.x = x1;
   Ret.y = y1;
   Ret.w = fig->w;
   Ret.h = fig->h;
   SDL_BlitSurface(fig,NULL,screen,&Ret);
}

/******************************************************
 *                        set                         *
 ******************************************************/
void picture::set(SDL_Surface* newPicture)
{
   fig = newPicture;
   setChanged();
}

/******************************************************
 *                         get                        *
 ******************************************************/
SDL_Surface* picture::get()
{
   return(fig);
}

