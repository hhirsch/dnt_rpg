/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2009 DNTeam <dnt@dnteam.org>
 
  This file is part of DccNiTghtmare.
 
  DccNiTghtmare is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  DccNiTghtmare is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with DccNiTghtmare.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "picture.h"

#include "../lang/translate.h"
#include <SDL/SDL_image.h>
#include <iostream>
using namespace std;

/******************************************************
 *                    Destructor                      *
 ******************************************************/
picture::~picture()
{
   if( (fig) && (deleteSurface) )
   {
      SDL_FreeSurface(fig);
   }

}

/******************************************************
 *                     Constructor                    *
 ******************************************************/
picture::picture(int x,int y,int w,int h,const char* arquivo, 
      SDL_Surface* surface): guiObject(surface)
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

   area.x = 0;
   area.y = 0;
   area.w = w;
   area.h = h;

   x1 = x;
   y1 = y;
   x2 = x+w;
   y2 = y+h;
   type = FARSO_OBJECT_PICTURE;

   deleteSurface = true;

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
      area.w = w;
      area.h = h;
   }
   else if( (w > 0) && (h > 0) )
   {
      fig = SDL_CreateRGBSurface(SDL_SWSURFACE,w,h,32,rmask,gmask,bmask,amask);
   }
   else
   {
      fig = NULL;
   }

   setAllVisible();
}

/******************************************************
 *               setSurfaceDelettion                  *
 ******************************************************/
void picture::setSurfaceDeletion(bool enable)
{
   deleteSurface = enable;
}

/******************************************************
 *                        draw                        *
 ******************************************************/
void picture::draw()
{
   if(fig == NULL)
   {
      return;
   }
   SDL_Rect Ret;
   Ret.x = x1;
   Ret.y = y1;
   SDL_BlitSurface(fig,&area,wSurface,&Ret);
}

/******************************************************
 *                        set                         *
 ******************************************************/
void picture::set(SDL_Surface* newPicture)
{
   fig = newPicture;
   area.x = 0;
   area.y = 0;
   area.w = (fig!=NULL)?fig->w:0;
   area.h = (fig!=NULL)?fig->h:0;
   setChanged();
}

/******************************************************
 *                         get                        *
 ******************************************************/
SDL_Surface* picture::get()
{
   return(fig);
}

/******************************************************
 *                  setVisibleArea                    *
 ******************************************************/
void picture::setVisibleArea(int xa, int ya, int xb, int yb)
{
   area.x = xa;
   area.y = ya;
   area.w = xb-xa;
   area.h = yb-ya;
}

/******************************************************
 *                   setAllVisible                    *
 ******************************************************/
void picture::setAllVisible()
{
   area.x = 0;
   area.y = 0;
   area.w = x2 - x1;
   area.h = y2 - y1;
}

