/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/
#ifndef _util_h
#define _util_h

#include <math.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL.h>
#include <GL/gl.h>
#include "../gui/farso.h"

/********************************************************************
 *                Character's moviments constants                   *
 ********************************************************************/
#define ANDAR  0.75    // The distance that normal character walks
#define GIRAR  2.5     // The turn velocity of normal character

/********************************************************************
 *                      Camera's constants                          *
 ********************************************************************/
#define DELTACAMERA    1.5            // The camera velocity
#define ZOOMMAXIMO     80             // Max value of Zoom
#define ZOOMMINIMO     280            // Min value of Zoom
#define FARVIEW        SQUARESIZE*500 // FarView constant
#define HALFFARVIEW    FARVIEW / 2.0  // Half FarView Value

/********************************************************************
 *                      Screen's constants                          *
 ********************************************************************/
#define SCREEN_X        800     // Screen Width
#define SCREEN_Y        600     // Screen Height

/********************************************************************
 *                        Math's constants                          *
 ********************************************************************/
#define TWOPI  2 * M_PI   
#define PID2   M_PI / 2
#define PID180 M_PI / 180.0
inline double deg2Rad(double x){return PID180 * x;} //deg to rad conversion


void atualizaCarga(SDL_Surface* img, GLuint* texturaTexto, 
                   GLuint texturaCarga, const char* texto,
                   GLdouble proj[16], GLdouble modl[16],GLint viewPort[4]);

int estaDentro(GLfloat min1[3], GLfloat max1[3],
               GLfloat min2[3], GLfloat max2[3],
               int inverso);

void rotTransBoundingBox(GLfloat orientacao, GLfloat X[4], GLfloat Z[4],
                         GLfloat varX, GLfloat varMinY, GLfloat varMaxY, 
                         GLfloat varZ,
                         GLfloat min[3], GLfloat max[3]);



#endif
