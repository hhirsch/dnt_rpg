/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2011 DNTeam <dnt@dnteam.org>
 
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

#include "util.h"

#include "../etc/dirs.h"

#include <iostream>
using namespace std;

/*********************************************************************
 *                   Loading Screen Atualization                     *
 *********************************************************************/
void showLoading(SDL_Surface* img, GLuint* texturaTexto, 
                 GLuint texturaCarga, const char* texto,
                 healthBar* progress)
{
   dirs dir;
   dntFont fnt;
   int centerY = SCREEN_Y / 2;
   int centerX = SCREEN_X / 2;
   int midW = img->w / 2;
   int midH = midW / 2;

   glClearColor(0,0,0,1);
   glClear ((GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
   color_Set(0,0,0,255);
   rectangle_Fill(img,0,0,img->w-1,img->h-1);
   color_Set(200,20,20,255);
   fnt.defineFont(dir.getRealFile(DNT_FONT_TIMES), 12);
   fnt.defineFontAlign(DNT_FONT_ALIGN_CENTER);
   fnt.write(img,128,0,texto);
   progress->draw();
   glGenTextures(1,texturaTexto);
   setTextureRGBA(img,*texturaTexto,false,GL_RGBA);

   draw2DMode();
   textureToScreen(texturaCarga, centerX-midW, centerY-midH,
                                 centerX+midW, centerY+midH,
                   2*midW,2*midH);
   textureToScreen(*texturaTexto, centerX-midW, centerY+midH+1,
                                  centerX+midW, centerY+midH+33,
                   2*midW,128);

   draw3DMode(OUTDOOR_FARVIEW);
   glFlush();
   SDL_GL_SwapBuffers();
   glDeleteTextures(1,texturaTexto);
   fnt.defineFontAlign(DNT_FONT_ALIGN_LEFT);
}

/*********************************************************************
 *                            actionInRange                          *
 *********************************************************************/
bool actionInRange(GLfloat posX, GLfloat posZ, 
                   GLfloat targX, GLfloat targZ,
                   GLfloat range)
{
   GLfloat dist = sqrt( (targX - posX) * (targX - posX) +
                        (targZ - posZ) * (targZ - posZ) );
   return( dist <= range);
}

/********************************************************************
 *                            getAngle                              *
 ********************************************************************/
GLfloat getAngle(GLfloat prvX, GLfloat prvZ, GLfloat curX, GLfloat curZ)
{
   GLfloat ax;
   GLfloat az;
   ax = fabs(prvX - curX);
   az = fabs(prvZ - curZ);
   GLfloat alpha;
   if( (ax != 0) )
   {
      alpha = ( (atan(fabs(az / ax)) / M_PI) * 180);
      if( (prvX > curX) && (prvZ < curZ) )
      {
         alpha += 180;
      }
      if( (prvX < curX) && (prvZ < curZ) )
      {
         alpha = -alpha;
      }
      if( (prvX > curX) && (prvZ > curZ) )
      {
         alpha = 180-alpha;
      }
      if(az == 0)
      {
         if(prvX < curX)
         {
            alpha = 0;
         }
         else
         {
            alpha = 180;
         }
      }

      return(alpha-90); /* -90 to correct model orientation*/

   }
   else
   {
      //alpha == 0 or 180 or no one!;
      if(prvZ < curZ)
      {
         return(180);
      }
      else
      {
         return(0);
      }
   }

   /* Shouldn't here */
   return(0);
}

/*********************************************************************
 *                               rotatePoint                         *
 *********************************************************************/
void rotatePoint(GLfloat pX, GLfloat pY, GLfloat pZ,
                 GLfloat angleX, GLfloat angleY, GLfloat angleZ,
                 GLfloat sinAngleX, GLfloat cosAngleX,
                 GLfloat sinAngleY, GLfloat cosAngleY,
                 GLfloat sinAngleZ, GLfloat cosAngleZ,
                 GLfloat& resX, GLfloat& resY, GLfloat& resZ)
{
   /* Set initial result (no angle!) */
   resX = pX;
   resY = pY;
   resZ = pZ;

   /* Rotate around Y axis */
   if(angleY != 0)
   {
      /* Calculate new point */
      resX = (pZ*sinAngleY) + (pX*cosAngleY);
      resZ = (pZ*cosAngleY) - (pX*sinAngleY);
      pX = resX;
      pZ = resZ;
   }

   /* Rotate around X axis */
   if(angleX != 0)
   {
      /* Calculate new point */
      resY = -(pZ*sinAngleX) + (pY*cosAngleX);
      resZ =(pZ*cosAngleX) + (pY*sinAngleX);
      pY = resY;
      pZ = resZ;
   }

   /* Rotate at Z axis */
   if(angleZ != 0)
   {
      /* Calculate new point */
      resX = -(pY*sinAngleZ) + (pX*cosAngleZ);
      resY = (pY*cosAngleZ) + (pX*sinAngleZ);
   }
}

/*********************************************************************
 *                               rotatePoint                         *
 *********************************************************************/
void rotatePoint(GLfloat srcX, GLfloat srcY, GLfloat srcZ, GLfloat theta,
                 GLfloat p1X, GLfloat p1Y, GLfloat p1Z, 
                 GLfloat p2X, GLfloat p2Y, GLfloat p2Z, 
                 GLfloat& resX, GLfloat& resY, GLfloat& resZ)
{
   GLfloat costheta,sintheta;
   GLfloat rX, rY, rZ;

   /* Calculate the axys vector */
   resX = 0; resY = 0; resZ = 0;
   rX = p2X - p1X;
   rY = p2Y - p1Y;
   rZ = p2Z - p1Z;
   srcX -= p1X;
   srcY -= p1Y;
   srcZ -= p1Z;

   /* Normalize the vector */
   normalize(rX, rY, rZ);

   /* Define the in and cos, to avoid calculating it all the time */
   costheta = cos(theta);
   sintheta = sin(theta);

   resX += (costheta + (1 - costheta) * rX * rX) * srcX;
   resX += ((1 - costheta) * rX * rY - rZ * sintheta) * srcY;
   resX += ((1 - costheta) * rX * rZ + rY * sintheta) * srcZ;

   resY += ((1 - costheta) * rX * rY + rZ * sintheta) * srcX;
   resY += (costheta + (1 - costheta) * rY * rY) * srcY;
   resY += ((1 - costheta) * rY * rZ - rX * sintheta) * srcZ;

   resZ += ((1 - costheta) * rX * rZ - rY * sintheta) * srcX;
   resZ += ((1 - costheta) * rY * rZ + rX * sintheta) * srcY;
   resZ += (costheta + (1 - costheta) * rZ * rZ) * srcZ;

   resX += p1X;
   resY += p1Y;
   resZ += p1Z;
}

/*********************************************************************
 *                               normalize                           *
 *********************************************************************/
void normalize (GLfloat& nx, GLfloat& ny, GLfloat& nz)
{
    // calculate the length of the vector
    GLfloat len = (GLfloat)(sqrt((nx * nx) + (ny * ny) + (nz * nz)));

    // avoid division by 0
    if (len == 0.0f)
        len = 1.0f;

    // reduce to unit size
    nx /= len;
    ny /= len;
    nz /= len;
}

/*********************************************************************
 *                               normalize                           *
 *********************************************************************/
void normalize(GLfloat& nx, GLfloat& ny)
{
    // calculate the length of the vector
    GLfloat len = (GLfloat)(sqrt((nx * nx) + (ny * ny)));

    // avoid division by 0
    if (len == 0.0f)
        len = 1.0f;

    // reduce to unit size
    nx /= len;
    ny /= len;
}

/*********************************************************************
 *                                normal                             *
 *********************************************************************/
void normal (GLfloat x1, GLfloat y1, GLfloat z1,
             GLfloat x2, GLfloat y2, GLfloat z2,
             GLfloat x3, GLfloat y3, GLfloat z3, 
             GLfloat& nx, GLfloat &ny, GLfloat& nz)
{
    GLfloat ax, ay, az , bx, by, bz;

    // calculate the vectors A and B
    // note that v[3] is defined with counterclockwise winding in mind
    // a
    ax = x1 - x2;
    ay = y1 - y2;
    az = z1 - z2;
    // b
    bx = x2 - x3;
    by = y2 - y3;
    bz = z2 - z3;

    // calculate the cross product and place the resulting vector
    // into the address specified by vertex_t *normal
    nx = (ay * bz) - (az * by);
    ny = (az * bx) - (ax * bz);
    nz = (ax * by) - (ay * bx);

    // normalize
    normalize(nx, ny, nz);
}

/*********************************************************************
 *                             draw2DMode                            *
 *********************************************************************/
void draw2DMode()
{
   /* Save the current 3D one */
   glPushMatrix();

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluOrtho2D(0.0, (GLdouble) SCREEN_X, 0.0, (GLdouble) SCREEN_Y);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

/*********************************************************************
 *                             draw3DMode                            *
 *********************************************************************/
void draw3DMode(float actualFarView)
{
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   gluPerspective(45.0, SCREEN_X / (float)SCREEN_Y, 1.0, actualFarView);
   glMatrixMode (GL_MODELVIEW);
   glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
   /* Reload the saved 3D One */
   glPopMatrix();
}

/*********************************************************************
 *                           readFrontBuffer                         *
 *********************************************************************/
SDL_Surface* readFrontBuffer()
{
   SDL_Surface* screen = NULL;

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

   /* Create the Screen */
   screen = SDL_CreateRGBSurface(SDL_SWSURFACE, SCREEN_X, SCREEN_Y, 32, 
                                 rmask, gmask, bmask, amask);

   /* Read screen from the buffer */
   glReadBuffer(GL_FRONT);
   glReadPixels(0, 0, SCREEN_X, SCREEN_Y, GL_RGBA, GL_UNSIGNED_BYTE, 
                screen->pixels);

   return(screen);
}

/*********************************************************************
 *                              screenshot                           *
 *********************************************************************/
bool screenshot(SDL_Surface* screen, string fileName, bool thumb)
{
   bool res;
   SDL_Surface* dest = NULL;
   int i,j;

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

   if(screen == NULL)
   {
      return(false);
   }

   /* Create the destiny surface */
   if(thumb)
   {
      dest = SDL_CreateRGBSurface(SDL_SWSURFACE, THUMB_X, THUMB_Y, 32, 
                                  rmask, gmask, bmask, amask);
   }
   else
   {
      dest = SDL_CreateRGBSurface(SDL_SWSURFACE, SCREEN_X, SCREEN_Y, 32, 
                                  rmask, gmask, bmask, amask);
   }

   /* invert the image Y or scale and invert it */
   if(!thumb)
   {
      /* Only invert the Y axys */
      for(i = 0; i < screen->w; i++)
      {
         for(j = 0; j < screen->h; j++)
         {
            pixel_Set(dest,i,j, pixel_Get(screen,i,screen->h-j-1));
         }
      }
   }
   else
   {
      int sumX = screen->w / THUMB_X,
          sumY = screen->h / THUMB_Y;
      /* Copy scaling and inverting the Y axys */
      for(i = 0; i < THUMB_X; i++)
      {
         for(j = 0; j < THUMB_Y; j++)
         {
            pixel_Set(dest,i,j, 
                      pixel_Get_Interpolate(screen, (i*sumX), 
                                            (screen->h - ((j-1)*sumY))));
         }
      }
   }
   
   res = (SDL_SaveBMP(dest, fileName.c_str()) == 0);
   
   SDL_FreeSurface(dest);

   return(res);
}


/*********************************************************************
 *                              screenshot                           *
 *********************************************************************/
bool screenshot(string fileName, bool thumb) 
{
   bool res;

   /* Get the current front buffer */
   SDL_Surface* screen = readFrontBuffer();
   
   /* Save the screenshot */
   res = screenshot(screen, fileName, thumb);

   /* Free the Surface */
   SDL_FreeSurface(screen);

   return(res);
}

/******************************************************************
 *                        replaceSpaces                           *
 ******************************************************************/
string replaceSpaces(string s)
{
   size_t found;
   string str = s;

   found = str.find_first_of(" \t");
   while(found != string::npos)
   {
      str[found]='_';
      found = str.find_first_of(" \t",found+1);
   }

   return(str);
}

