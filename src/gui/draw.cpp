/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */


#include "draw.h"
#include <SDL/SDL_opengl.h>
#include <GL/glu.h>


int R;  /**< Active Red color */
int G;  /**< Active Green Color */
int B;  /**< Active Blue Color */


/******************************************************************
 *                            color_Set                           *
 ******************************************************************/
void color_Set(Uint8 Ri, Uint8 Gi, Uint8 Bi)
{
   R = Ri;
   G = Gi;
   B = Bi;
}

/******************************************************************
 *                            color_Get                           *
 ******************************************************************/
void color_Get(Uint8 *Ri, Uint8 *Gi, Uint8 *Bi)
{
   *Ri = R;
   *Gi = G;
   *Bi = B;
}

/******************************************************************
 *                            pixel_Set                           *
 ******************************************************************/
void pixel_Set(SDL_Surface *screen, int x, int y)
{
    Uint32 color = SDL_MapRGB(screen->format, R, G, B);

    if((x>screen->w-1) || (y>screen->h-1) || (x<0) || (y<0))
       return;

    if ( SDL_MUSTLOCK(screen) ) {
        if ( SDL_LockSurface(screen) < 0 ) {
            return;
        }
    }
    switch (screen->format->BytesPerPixel) {
        case 1: { /* Assumindo 8-bpp */
            Uint8 *bufp;

            bufp = (Uint8 *)screen->pixels + y*screen->pitch + x;
            *bufp = color;
        }
        break;

        case 2: { /* Provavelmente 15-bpp ou 16-bpp */
            Uint16 *bufp;

            bufp = (Uint16 *)screen->pixels + y*screen->pitch/2 + x;
            *bufp = color;
        }
        break;

        case 3: { /* mode 24-bpp lento,normalmente não usado */
            Uint8 *bufp;

            bufp = (Uint8 *)screen->pixels + y*screen->pitch + x;
            *(bufp+screen->format->Rshift/8) = R;
            *(bufp+screen->format->Gshift/8) = G;
            *(bufp+screen->format->Bshift/8) = B;
        }
        break;

        case 4: { /* Provavelmente 32-bpp */
            Uint32 *bufp;

            bufp = (Uint32 *)screen->pixels + y*screen->pitch/4 + x;
            *bufp = color;
        }
        break;
    }
    if ( SDL_MUSTLOCK(screen) ) 
    {
       SDL_UnlockSurface(screen);
    }
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
                       int Ri,int Gi, int Bi)
{
    Uint8 Ra,Ga,Ba;
    color_Get(&Ra,&Ga,&Ba);
    line_Draw(screen,x1,y1,x2,y1);
    line_Draw(screen,x1,y1,x1,y2);
    color_Set(Ri,Gi,Bi);
    line_Draw(screen,x1,y2,x2,y2);
    line_Draw(screen,x2,y1,x2,y2);
    color_Set(Ra,Ga,Ba);
}

/******************************************************************
 *                            pixel_Get                           *
 ******************************************************************/
Uint32 pixel_Get(SDL_Surface *surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
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
   Uint32 cor = SDL_MapRGB(screen->format,R,G,B);
   if ( SDL_MUSTLOCK(screen) ) 
   {
        if ( SDL_LockSurface(screen) < 0 ) 
        {
            return;
        }
    }
      SDL_FillRect(screen,&ret,cor);
   if ( SDL_MUSTLOCK(screen) ) 
   {
        SDL_UnlockSurface(screen);
   }
}

/******************************************************************
 *                         rectangle_Oval                         *
 ******************************************************************/
void rectangle_Oval(SDL_Surface *screen, int x1, int y1, int x2, int y2, 
                    int Ri, int Gi, int Bi)
{
   Uint8 Ra,Ga,Ba;
   color_Get(&Ra,&Ga,&Ba);
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
   color_Set(Ri,Gi,Bi);
   line_Draw(screen,x1,y2-2,x1+2,y2);
   line_Draw(screen,x1+2,y2,x2-2,y2);
   line_Draw(screen,x2-2,y2,x2,y2-2);
   line_Draw(screen,x2,y2-2,x2,y1+2);
   line_Draw(screen,x2-2,y1,x2,y1+2);
   color_Set(Ra,Ga,Ba);
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
void textureToScreen(GLuint texturaID, GLdouble proj[16],GLdouble modl[16], 
                     GLint viewPort[4],int xa, int ya, int xb, int yb, 
                     double depth)
{
   GLdouble x1,y1,z1, x2,y2,z2, x3,y3,z3, x4,y4,z4;

   GLfloat xOverlay = (GLfloat) (xb-xa+1)/(GLfloat)smallestPowerOfTwo(xb-xa+1);
   GLfloat yOverlay = (GLfloat) (yb-ya+1)/(GLfloat)smallestPowerOfTwo(yb-ya+1);
   
   gluUnProject(xa,(599-ya), depth, modl, proj, viewPort, &x1, &y1, &z1);
   gluUnProject(xa,(599-yb-1),depth, modl, proj, viewPort, &x2, &y2, &z2);
   gluUnProject(xb+1, (599-yb-1), depth, modl, proj, viewPort, 
                &x3, &y3, &z3);
   gluUnProject(xb+1,(599-ya),depth, modl, proj, viewPort, &x4, &y4, &z4);

   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, texturaID);

   glBegin(GL_QUADS);
      glTexCoord2f(0,0);
      glVertex3f(x1, y1, z1);
	
      glTexCoord2f(xOverlay,0);
      glVertex3f(x4, y4, z4);
	
      glTexCoord2f(xOverlay,yOverlay);
      glVertex3f(x3, y3, z3);
	
      glTexCoord2f(0,yOverlay);
      glVertex3f(x2, y2, z2);
   glEnd();

   glDisable(GL_TEXTURE_2D);
}

/*********************************************************************
 *                             setTexture                            *
 *********************************************************************/
void setTexture(SDL_Surface* img, GLuint* textID)
{
   glGenTextures(1,textID);
   glBindTexture(GL_TEXTURE_2D,*textID);

   if( ( (img->w & (img->w - 1)) != 0 ) ||
       ( (img->h & (img->h - 1)) != 0 )  )
   {
      /* Convert to Power of Two */
      SDL_Surface* tmp = SDL_CreateRGBSurface(SDL_HWSURFACE,
                       smallestPowerOfTwo(img->w),
                       smallestPowerOfTwo(img->h),32,
                       0x000000FF,0x0000FF00,0x00FF0000,0xFF000000);
      
      SDL_BlitSurface(img, NULL, tmp, NULL);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tmp->w, tmp->h, 
                                  0, GL_RGBA, GL_UNSIGNED_BYTE, 
                                  tmp->pixels);
      SDL_FreeSurface(tmp);
   }
   else
   {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img->w, img->h, 
                                  0, GL_RGB, GL_UNSIGNED_BYTE, 
                                  img->pixels);
   }
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
}

/******************************************************************
 *                          setTextureRGBA                        *
 ******************************************************************/
void setTextureRGBA(SDL_Surface* img, GLuint* textID)
{
   glGenTextures(1,textID);
   glBindTexture(GL_TEXTURE_2D,*textID);

   if( ( (img->w & (img->w - 1)) != 0 ) ||
       ( (img->h & (img->h - 1)) != 0 )  )
   {
      /* Convert to Power of Two */
      SDL_Surface* tmp = SDL_CreateRGBSurface(SDL_HWSURFACE,
                       smallestPowerOfTwo(img->w),
                       smallestPowerOfTwo(img->h),32,
                       0x000000FF,0x0000FF00,0x00FF0000,0xFF000000);
      
      SDL_BlitSurface(img, NULL, tmp, NULL);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tmp->w, tmp->h, 
                                  0, GL_RGBA, GL_UNSIGNED_BYTE, 
                                  tmp->pixels);
      SDL_FreeSurface(tmp);
   }
   else
   {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->w, img->h, 
                                  0, GL_RGBA, GL_UNSIGNED_BYTE, 
                                  img->pixels);
   }
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
}

