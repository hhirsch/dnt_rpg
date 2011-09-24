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

#ifndef _farso_draw_h
#define _farso_draw_h

/*! \file draw.h, where defined Surface draw functions. */
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#ifdef __APPLE__
   #include <OpenGL/glu.h>
#else
   #include <GL/glu.h>
#endif
#include <stdlib.h>
#include <math.h>

#define RAIZ3D2  0.8660254 /**< Sqrt(3) / 2 */

#ifdef __APPLE__
   #define DNT_IMAGE_FORMAT    GL_BGR
   #define DNT_IMAGE_FORMAT_A  GL_BGRA
#else
   #define DNT_IMAGE_FORMAT    GL_RGB
   #define DNT_IMAGE_FORMAT_A  GL_RGBA
#endif


/********************************************************************
 *                      Screen's constants                          *
 ********************************************************************/
extern int SCREEN_X;     /**< Current Screen Width */
extern int SCREEN_Y;     /**< Current Screen Height */


/*! \note All Draws are made if activeColor */

/*! Define active Color
 * \param Ri -> Red, from 0 to 255
 * \param Gi -> Green, from 0 to 255
 * \param Bi -> Blue, from 0 to 255 
 * \param Ai -> Alpha, from 0 to 255 */
void color_Set(Uint8 Ri, Uint8 Gi, Uint8 Bi, Uint8 Ai);

/*! Get the Active Color
 * \param Ri -> Red
 * \param Gi -> Green
 * \param Bi -> Blue
 * \param Ai -> Alpha */
void color_Get(Uint8 *Ri, Uint8 *Gi, Uint8 *Bi, Uint8 *Ai);

/*! Set the surface (x,y) pixel color
 * \param screen -> surface to draw
 * \param x -> x coordinate of the pixel
 * \param y -> y coordinate of the pixel */
void pixel_Set(SDL_Surface *screen, int x, int y);

/*! Set the surface (x,y) pixel color
 * \param screen -> surface to draw
 * \param x -> x coordinate of the pixel
 * \param y -> y coordinate of the pixel
 * \param red -> red  value
 * \param green -> green value
 * \param blue -> blue value
 * \param alpha -> alpha value */
void pixel_Set(SDL_Surface* screen, int x, int y, 
               int red, int green, int blue, int alpha);

/*! Set the surface (x,y) pixel color
 * \param screen -> surface to draw 
 * \param x -> x coordinate of the pixel
 * \param y -> y coordinate of the pixel
 * \param color -> color to set pixel */
void pixel_Set(SDL_Surface* screen, int x, int y, Uint32 color);

/*! Get the surface (x,y) pixel color
 * \param surface -> surface to draw
 * \param x -> x coordinate of the pixel
 * \param y -> y coordinate of the pixel 
 * \return -> (x,y) surface color */
Uint32 pixel_Get(SDL_Surface *surface, int x, int y);

/*! Same as pixel_Get, but interpolate with nearest neighbors
 * \param surface -> surface to draw
 * \param x -> x coordinate of the pixel
 * \param y -> y coordinate of the pixel 
 * \return -> (x,y) surface color */
Uint32 pixel_Get_Interpolate(SDL_Surface *surface, int x, int y);

/*! Draw a line on the surface
 * \param screen -> surface to draw to
 * \param x1 -> x initial coordinate
 * \param y1 -> y initial coordinate
 * \param x2 -> x final coordinate
 * \param y2 -> y final coordinate */
void line_Draw(SDL_Surface *screen, int x1, int y1, int x2, int y2);

/*! Draw a rectangle on the surface
 * \param screen -> surface to draw to
 * \param x1 -> x initial coordinate
 * \param y1 -> y initial coordinate
 * \param x2 -> x final coordinate
 * \param y2 -> y final coordinate */
void rectangle_Draw(SDL_Surface *screen, int x1, int y1, int x2, int y2);

/*! Draw and Fill a rectangle on surface
 * \param screen -> surface to draw to
 * \param x1 -> x initial coordinate
 * \param y1 -> y initial coordinate
 * \param x2 -> x final coordinate
 * \param y2 -> y final coordinate */
void rectangle_Fill(SDL_Surface *screen, int x1, int y1, int x2, int y2);

/*! Draws an Oval rectangle (usefull for buttons)
 * \param screen -> surface to draw to
 * \param x1 -> x initial coordinate
 * \param y1 -> y initial coordinate
 * \param x2 -> x final coordinate
 * \param y2 -> y final coordinate
 * \param Ri -> second Red Color
 * \param Gi -> second Green Color
 * \param Bi -> second Blue Color
 * \param Ai -> second Alpha Color */
void rectangle_Oval(SDL_Surface *screen, int x1, int y1, int x2, int y2, 
                    int Ri, int Gi, int Bi, int Ai);

/*! Draws a 2 colors rectangle on the surface (usefull for buttons)  
 * \param screen -> surface to draw to
 * \param x1 -> x initial coordinate
 * \param y1 -> y initial coordinate
 * \param x2 -> x final coordinate
 * \param y2 -> y final coordinate
 * \param Ri -> second Red Color
 * \param Gi -> second Green Color
 * \param Bi -> second Blue Color 
 * \param Ai -> second Alpha Color */
void rectangle_2Colors(SDL_Surface *screen, int x1, int y1, int x2, int y2, 
                      int Ri,int Gi, int Bi, int Ai);

/*! Draw a Circle to the surface
 * \param screen -> surface where draw the cricle
 * \param xC -> circle X center coordinate
 * \param yC -> circle Y center coordinate
 * \param r  -> circle radius */
void circle_Draw(SDL_Surface *screen, int xC, int yC, int r);

/*! Draw the Texture to the Screen Coordinates 
 * \param texturaID -> GL Identifier of the texture
 * \param xa -> x1 screen coordinate
 * \param ya -> y1 screen coordinate
 * \param xb -> x2 screen coordinate
 * \param yb -> y2 screen coordinate
 * \param width -> texture width
 * \param height -> texture height */
void textureToScreen(GLuint texturaID, int xa, int ya,int xb, int yb,
                     int width, int height);

/*! Return the smallest power of two greater or equal to the number
 * \param num -> bases number 
 * \return -> smallest power of two greater or equal to the number */
int smallestPowerOfTwo(int num);

/*! Load the GL texture based on the surface
 * \param img -> surface to load as texture
 * \param textID -> texture Identifier loaded
 * \param mipMaps-> if will use mipMaps */
void setTexture(SDL_Surface* img, GLuint textID, bool mipMaps=false);

/*! Load the GL RGBA texture based on the surface
 * \param img -> surface to load as texture
 * \param textID -> texture Identifier loaded
 * \param mipMaps -> if will use mipMaps
 * \param format -> workaround for SDL_Image broken on MacOSX */
void setTextureRGBA(SDL_Surface* img, GLuint textID, bool mipMaps=false,
                    GLint format=DNT_IMAGE_FORMAT_A);


#endif

