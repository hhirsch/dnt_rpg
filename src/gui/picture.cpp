/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */

#include "picture.h"

#include <SDL/SDL_image.h>
#include <libintl.h>
#include <iostream>
using namespace std;

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
   /* Define Machine Bit Order */
   Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
   rmask = 0xff000000;
   gmask = 0x00ff0000;
   bmask = 0x0000ff00;
   amask = 0x000000ff;
#else
   rmask = 0x000000ff;
   gmask = 0x0000ff00;
   bmask = 0x00ff0000;
   amask = 0xff000000;
#endif


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
         cerr << "Can't open image: " << arquivo << endl;
         return;
      }

            //fig = img;
      fig = SDL_CreateRGBSurface(SDL_SWSURFACE,
                                 img->w,img->h,32,
                                 rmask, gmask, bmask, amask);
      SDL_SetAlpha(img, 0,0);
      SDL_BlitSurface(img,NULL,fig,NULL);
      SDL_FreeSurface(img);
      x2 = x1 + fig->w;
      y2 = y1 + fig->h;
   }
   else if( (w > 0) && (h > 0) )
   {
      fig = SDL_CreateRGBSurface(SDL_SWSURFACE,w,h,32,rmask,gmask,bmask,amask);
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

