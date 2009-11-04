/* 
  DccNiTghtmare: a satiric post-apocalyptical RPG.
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

#include "draw.h"
#include <SDL/SDL_opengl.h>
#include <GL/glu.h>


int SCREEN_X; /**< Current Screen Width */
int SCREEN_Y; /**< Scurrent Screen Height */
int R;  /**< Active Red color */
int G;  /**< Active Green Color */
int B;  /**< Active Blue Color */
int A;  /**< Active Alpha Color */

/******************************************************************
 *                            color_Set                           *
 ******************************************************************/
void color_Set(Uint8 Ri, Uint8 Gi, Uint8 Bi, Uint8 Ai)
{
   R = Ri;
   G = Gi;
   B = Bi;
   A = Ai;
}

/******************************************************************
 *                            color_Get                           *
 ******************************************************************/
void color_Get(Uint8 *Ri, Uint8 *Gi, Uint8 *Bi, Uint8* Ai)
{
   *Ri = R;
   *Gi = G;
   *Bi = B;
   *Ai = A;
}

/******************************************************************
 *                            pixel_Set                           *
 ******************************************************************/
void pixel_Set(SDL_Surface* screen, int x, int y, 
               int red, int green, int blue, int alpha)
{
   int bpp = screen->format->BytesPerPixel;
   Uint32 color;

   /* Map RGB(A) color to the SDL Color */
   if(bpp == 4)
   {
      color = SDL_MapRGBA(screen->format, red, green, blue, alpha);
   }
   else 
   {
      color = SDL_MapRGB(screen->format, red, green, blue);
   }
   pixel_Set(screen, x, y, color);
}

/******************************************************************
 *                            pixel_Set                           *
 ******************************************************************/
void pixel_Set(SDL_Surface* screen, int x, int y, Uint32 color)
{
   int bpp = screen->format->BytesPerPixel;

   /* Verify Limits */
   if((x>screen->w-1) || (y>screen->h-1) || (x<0) || (y<0))
   {
       return;
   }

   if ( SDL_MUSTLOCK(screen) ) 
   {
      if ( SDL_LockSurface(screen) < 0 ) 
      {
         return;
      }
   }

   /* Here p is the address to the pixel we want to set */
   Uint8 *p = (Uint8 *)screen->pixels + y * screen->pitch + x * bpp;

   switch(bpp) 
   {
      case 1:
      {
         *p = color;
      }
      break;

      case 2:
      {
         *(Uint16 *)p = color;
      }
      break;

      case 3:
      {
         if(SDL_BYTEORDER == SDL_BIG_ENDIAN) 
         {
            p[0] = (color >> 16) & 0xff;
            p[1] = (color >> 8) & 0xff;
            p[2] = color & 0xff;
         } 
         else 
         {
            p[0] = color & 0xff;
            p[1] = (color >> 8) & 0xff;
            p[2] = (color >> 16) & 0xff;
         }
      }
      break;

      case 4:
      {
         *(Uint32 *)p = color;
      }
      break;
   }

   if ( SDL_MUSTLOCK(screen) ) 
   {
      SDL_UnlockSurface(screen);
   }
}

/******************************************************************
 *                            pixel_Set                           *
 ******************************************************************/
void pixel_Set(SDL_Surface *screen, int x, int y)
{
    pixel_Set(screen, x, y, R, G, B, A);
}

/******************************************************************
 *                              round                             *
 ******************************************************************/
inline int round(const float a)
{
   /* Used to the line draw function */
   return (int)(a+0.5);
}

/******************************************************************
 *                            line_Draw                           *
 ******************************************************************/
void line_Draw(SDL_Surface *screen, int x1, int y1, int x2, int y2)
{
   int dx = x2 - x1;
   int dy = y2 - y1;
   int passos, k;
   float xInc, yInc, x = x1, y = y1;
   if(fabs(dx) > fabs(dy))
      passos = (int) fabs(dx);
   else
      passos = (int) fabs(dy);
   xInc = (float) (dx) / (float) (passos);
   yInc = (float) (dy) / (float) (passos);
   pixel_Set(screen, round(x), round(y));
   for(k=0;k<passos;k++)
   {
      x+=xInc;
      y+=yInc;
      pixel_Set(screen, round(x), round(y));
   }
}

/******************************************************************
 *                         rectangle_Draw                         *
 ******************************************************************/
void rectangle_Draw(SDL_Surface *screen, int x1, int y1, int x2, int y2)
{
    line_Draw(screen,x1,y1,x2,y1);
    line_Draw(screen,x1,y2,x2,y2);
    line_Draw(screen,x1,y1,x1,y2);
    line_Draw(screen,x2,y1,x2,y2);
}

/******************************************************************
 *                        rectangle_2Colors                       *
 ******************************************************************/
void rectangle_2Colors(SDL_Surface *screen, int x1, int y1, int x2, int y2, 
                       int Ri,int Gi, int Bi, int Ai)
{
    Uint8 Ra,Ga,Ba, Aa;
    color_Get(&Ra,&Ga,&Ba, &Aa);
    line_Draw(screen,x1,y1,x2,y1);
    line_Draw(screen,x1,y1,x1,y2);
    color_Set(Ri,Gi,Bi,Ai);
    line_Draw(screen,x1,y2,x2,y2);
    line_Draw(screen,x2,y1,x2,y2);
    color_Set(Ra,Ga,Ba,Aa);
}

/******************************************************************
 *                            pixel_Get                           *
 ******************************************************************/
Uint32 pixel_Get_Interpolate(SDL_Surface *surface, int x, int y)
{
   Uint32 resR=0, resG=0, resB=0;
   Uint8 r,g,b;

   SDL_GetRGB(pixel_Get(surface, x, y), surface->format, &r, &g, &b);
   resR += r; resB += b; resG += g;
   SDL_GetRGB(pixel_Get(surface, x-1, y), surface->format, &r, &g, &b);
   resR += r; resB += b; resG += g;
   SDL_GetRGB(pixel_Get(surface, x+1, y), surface->format, &r, &g, &b);
   resR += r; resB += b; resG += g;
   SDL_GetRGB(pixel_Get(surface, x, y-1), surface->format, &r, &g, &b);
   resR += r; resB += b; resG += g;
   SDL_GetRGB(pixel_Get(surface, x, y+1), surface->format, &r, &g, &b);
   resR += r; resB += b; resG += g;

   SDL_GetRGB(pixel_Get(surface, x-1, y-1), surface->format, &r, &g, &b); 
   resR += r; resB += b; resG += g;
   SDL_GetRGB(pixel_Get(surface, x-1, y+1), surface->format, &r, &g, &b);
   resR += r; resB += b; resG += g;
   SDL_GetRGB(pixel_Get(surface, x+1, y-1), surface->format, &r, &g, &b);
   resR += r; resB += b; resG += g;
   SDL_GetRGB(pixel_Get(surface, x+1, y+1), surface->format, &r, &g, &b);
   resR += r; resB += b; resG += g;


   resR = resR / 9;
   resG = resG / 9;
   resB = resB / 9;

   return(SDL_MapRGBA(surface->format, resR, resG, resB, 255));
}

/******************************************************************
 *                            pixel_Get                           *
 ******************************************************************/
Uint32 pixel_Get(SDL_Surface *surface, int x, int y)
{
   /* Verify Limits */
   if((x > surface->w-1) || (y > surface->h-1) || (x<0) || (y<0))
   {
      return(0);
   }

   int bpp = surface->format->BytesPerPixel;
   /* Here p is the address to the pixel we want to retrieve */
   Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

   switch(bpp) 
   {
      case 1:
         return *p;

      case 2:
         return *(Uint16 *)p;

      case 3:
         if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
         else
            return p[0] | p[1] << 8 | p[2] << 16;

      case 4:
         return *(Uint32 *)p;

      default:
         return 0;       /* shouldn't happen, but avoids warnings */
   }
}

/******************************************************************
 *                         rectangle_Fill                         *
 ******************************************************************/
void rectangle_Fill(SDL_Surface *screen, int x1, int y1, int x2, int y2)
{
   SDL_Rect ret;
   ret.x = x1;
   ret.y = y1;
   ret.w = (x2 - x1)+1;
   ret.h = (y2 - y1)+1;

   int bpp = screen->format->BytesPerPixel;
   Uint32 color;

   if(bpp == 4)
   {
      color = SDL_MapRGBA(screen->format, R, G, B, A);
   }
   else 
   {
      color = SDL_MapRGB(screen->format, R, G, B);
   }

   if ( SDL_MUSTLOCK(screen) ) 
   {
      if ( SDL_LockSurface(screen) < 0 ) 
      {
         return;
      }
   }

   SDL_FillRect(screen,&ret,color);

   if ( SDL_MUSTLOCK(screen) ) 
   {
      SDL_UnlockSurface(screen);
   }
}

/******************************************************************
 *                         rectangle_Oval                         *
 ******************************************************************/
void rectangle_Oval(SDL_Surface *screen, int x1, int y1, int x2, int y2, 
                    int Ri, int Gi, int Bi, int Ai)
{
   Uint8 Ra,Ga,Ba,Aa;
   color_Get(&Ra,&Ga,&Ba,&Aa);
   if (x2<x1)
   { 
      int aux = x1;
      x1 = x2;
      x2 = aux;
   }
   if (y2<y1) 
   {
      int aux = y1;
      y1 = y2;
      y2 = aux;
   }
   line_Draw(screen,x1,y1+2,x1+2,y1);
   line_Draw(screen,x1+2,y1,x2-2,y1);
   line_Draw(screen,x1,y1+2,x1,y2-2);
   color_Set(Ri,Gi,Bi,Ai);
   line_Draw(screen,x1,y2-2,x1+2,y2);
   line_Draw(screen,x1+2,y2,x2-2,y2);
   line_Draw(screen,x2-2,y2,x2,y2-2);
   line_Draw(screen,x2,y2-2,x2,y1+2);
   line_Draw(screen,x2-2,y1,x2,y1+2);
   color_Set(Ra,Ga,Ba,Aa);
}

/******************************************************************
 *                          circle_Draw                           *
 ******************************************************************/
void circle_Draw(SDL_Surface *screen,int xC, int yC, int r)
{
   int x=0, y=r,u=1,v=2*r-1,E=0;
   while(x < y)
   {
     pixel_Set(screen,xC+x, yC+y);
     pixel_Set(screen,xC+y, yC-x);
     pixel_Set(screen,xC-x, yC-y);
     pixel_Set(screen,xC-y, yC+x);
     x++; E+= u; u+=2;
     if (v < 2 * E)
     {
         y--;
         E -= v;
         v -= 2;
      }
      if (x > y)
         break;
      pixel_Set(screen,xC+y, yC+x);
      pixel_Set(screen,xC+x, yC-y);
      pixel_Set(screen,xC-y, yC-x);
      pixel_Set(screen,xC-x, yC+y);
   }
}

/******************************************************************
 *                      smallestPowerOfTwo                        *
 ******************************************************************/
int smallestPowerOfTwo(int num)
{
   int i = 2;
   while(i < num)
   {
      i *= 2;
   }
   return(i);
}

/******************************************************************
 *                         textureToScreen                        *
 ******************************************************************/
void textureToScreen(GLuint texturaID, int xa, int ya, int xb, int yb, 
                     int width, int height)
{
   /* Calculate texture overlay */
   GLfloat xOverlay = (GLfloat) (width)/(GLfloat)smallestPowerOfTwo(width);
   GLfloat yOverlay = (GLfloat) (height)/(GLfloat)smallestPowerOfTwo(height);
   
   /* Set as GL 2D Coordinates */
   ya = SCREEN_Y - ya;
   yb = SCREEN_Y - yb;

   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, texturaID);

   glBegin(GL_QUADS);
      glTexCoord2f(0,0);
      glVertex3f(xa, ya, 0);
	
      glTexCoord2f(xOverlay,0);
      glVertex3f(xb, ya, 0);
	
      glTexCoord2f(xOverlay,yOverlay);
      glVertex3f(xb, yb, 0);
	
      glTexCoord2f(0,yOverlay);
      glVertex3f(xa, yb, 0);
   glEnd();

   glDisable(GL_TEXTURE_2D);
}

/*********************************************************************
 *                             setTexture                            *
 *********************************************************************/
void setTexture(SDL_Surface* img, GLuint textID, bool mipMaps)
{
   SDL_Surface* tmp = NULL;
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

   if(img->format->BytesPerPixel == 4)
   {
      setTextureRGBA(img, textID, mipMaps);
      return;
   }

   glBindTexture(GL_TEXTURE_2D,textID);

   if( ( (img->w & (img->w - 1)) != 0 ) ||
       ( (img->h & (img->h - 1)) != 0 )  )
   {
      /* Convert to Power of Two */
      tmp = SDL_CreateRGBSurface(SDL_SWSURFACE,
                       smallestPowerOfTwo(img->w),
                       smallestPowerOfTwo(img->h),32,
                       rmask, gmask, bmask, amask);
      
      SDL_BlitSurface(img, NULL, tmp, NULL);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tmp->w, tmp->h, 
                                  0, GL_RGBA, GL_UNSIGNED_BYTE, 
                                  tmp->pixels);      
   }
   else
   {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img->w, img->h, 
                                  0, GL_RGB, GL_UNSIGNED_BYTE, 
                                  img->pixels);
   }

   if(mipMaps)
   {
      glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

      glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,
            GL_LINEAR_MIPMAP_LINEAR );
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

      if(tmp)
      {
         gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, tmp->w,
               tmp->h, GL_RGBA, GL_UNSIGNED_BYTE,
               tmp->pixels );
      }
      else
      {
         gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, img->w,
               img->h, GL_RGB, GL_UNSIGNED_BYTE,
               img->pixels );
      }
   }
   else
   {
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
   }

   /* Free tmp as needed */
   if(tmp != NULL)
   {
      SDL_FreeSurface(tmp);
   }
}

/******************************************************************
 *                          setTextureRGBA                        *
 ******************************************************************/
void setTextureRGBA(SDL_Surface* img, GLuint textID, bool mipMaps)
{
   SDL_Surface* tmp = NULL;
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

   glBindTexture(GL_TEXTURE_2D,textID);

   /* Convert to Power of Two */
   if( ( (img->w & (img->w - 1)) != 0 ) ||
       ( (img->h & (img->h - 1)) != 0 )  )
   {
      /* Gamb is needed to copy the alpha channel from source to destiny
       * disabling the SRC_ALPHA from the source (but not really from the
       * source to avoid colatheral effects on latter blits with img.) */
      SDL_Surface* gamb = SDL_CreateRGBSurfaceFrom(img->pixels, 
                                                   img->w, img->h, 
                                                   img->format->BitsPerPixel,
                                                   img->pitch, 
                                                   rmask, gmask, bmask,amask);

      tmp = SDL_CreateRGBSurface(SDL_SWSURFACE,
                       smallestPowerOfTwo(img->w),
                       smallestPowerOfTwo(img->h),32,
                       rmask, gmask, bmask, amask);

      SDL_SetAlpha(gamb, 0, 0);
      SDL_BlitSurface(gamb, NULL, tmp, NULL);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tmp->w, tmp->h, 
                                  0, GL_RGBA, GL_UNSIGNED_BYTE, 
                                  tmp->pixels);
      SDL_FreeSurface(gamb);
   }
   else
   {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->w, img->h, 
                                  0, GL_RGBA, GL_UNSIGNED_BYTE, 
                                  img->pixels);
      tmp = img;
   }

   /* Generate mipMaps */
   if(mipMaps)
   {
      glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

      glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,
            GL_LINEAR_MIPMAP_LINEAR );
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


      gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, tmp->w,
            tmp->h, GL_RGBA, GL_UNSIGNED_BYTE,
            tmp->pixels );
   }
   else
   {
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
   }

   /* Free the tmp surface, if needed */
   if( (tmp != NULL) && (tmp != img))
   {
      SDL_FreeSurface(tmp);
   }
}


