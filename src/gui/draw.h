/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */


#ifndef _desenho_h
#define _desenho_h

/*! \file draw.h, where defined Surface draw functions. */
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <math.h>

#define RAIZ3D2  0.8660254 /**< Sqrt(3) / 2 */

/*! \note All Draws are made if activeColor */

/*! Define active Color
 * \param Ri -> Red, from 0 to 255
 * \param Gi -> Green, from 0 to 255
 * \param Bi -> Blue, from 0 to 255 */
void color_Set(Uint8 Ri, Uint8 Gi, Uint8 Bi);

/*! Define the active alpha
 * \param Ai -> Alpha, from 0 to 255 */
void color_Alpha(Uint8 Ai);

/*! Gets the Active Color
 * \param Ri -> Red
 * \param Gi -> Green
 * \param Bi -> Blue */
void color_Get(Uint8 *Ri, Uint8 *Gi, Uint8 *Bi);

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

/*! Get the surface (x,y) pixel color
 * \param surface -> surface to draw
 * \param x -> x coordinate of the pixel
 * \param y -> y coordinate of the pixel 
 * \return -> (x,y) surface color */
Uint32 pixel_Get(SDL_Surface *surface, int x, int y);

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
 * \param Bi -> second Blue Color */
void rectangle_Oval(SDL_Surface *screen, int x1, int y1, int x2, int y2, 
                    int Ri, int Gi, int Bi);

/*! Draws a 2 colors rectangle on the surface (usefull for buttons)  
 * \param screen -> surface to draw to
 * \param x1 -> x initial coordinate
 * \param y1 -> y initial coordinate
 * \param x2 -> x final coordinate
 * \param y2 -> y final coordinate
 * \param Ri -> second Red Color
 * \param Gi -> second Green Color
 * \param Bi -> second Blue Color */
void rectangle_2Colors(SDL_Surface *screen, int x1, int y1, int x2, int y2, 
                      int Ri,int Gi, int Bi);

/*! Draw a Circle to the surface
 * \param screen -> surface where draw the cricle
 * \param xC -> circle X center coordinate
 * \param yC -> circle Y center coordinate
 * \param r  -> circle radius */
void circle_Draw(SDL_Surface *screen, int xC, int yC, int r);

/*! Draw the Texture to the Screen Coordinates 
 * \param texturaID -> GL Identifier of the texture
 * \param proj -> the projection matrix
 * \param modl -> the model view matrix
 * \param viewPort -> the viewPort vector
 * \param xa -> x1 screen coordinate
 * \param ya -> y1 screen coordinate
 * \param xb -> x2 screen coordinate
 * \param yb -> y2 screen coordinate
 * \param depth -> screen depth of the texture */
void textureToScreen(GLuint texturaID, GLdouble proj[16],GLdouble modl[16], 
                     GLint viewPort[4], int xa, int ya,int xb, int yb,
                     double depth);

/*! Return the smallest power of two greater or equal to the number
 * \param num -> bases number 
 * \return -> smallest power of two greater or equal to the number */
int smallestPowerOfTwo(int num);

/*! Load the GL texture based on the surface
 * \param img -> surface to load as texture
 * \param textID -> texture Identifier loaded */
void setTexture(SDL_Surface* img, GLuint* textID);

/*! Load the GL RGBA texture based on the surface
 * \param img -> surface to load as texture
 * \param textID -> texture Identifier loaded*/
void setTextureRGBA(SDL_Surface* img, GLuint* textID);


#endif

