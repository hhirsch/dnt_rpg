/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/
#ifndef _util_h
#define _util_h

/*! \file util.h
 * This file contains util definitions and functions to the engine 
 * functionality. */

#include <math.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include "../gui/farso.h"

/********************************************************************
 *                Character's moviments constants                   *
 ********************************************************************/
#define ANDAR  0.75    /**< The distance that normal character walks */
#define GIRAR  2.5     /**< The turn velocity of normal character */
#define WALK_PER_MOVE_ACTION 60 /**< Distance that can be walked per move */

/********************************************************************
 *                      Camera's constants                          *
 ********************************************************************/
#define DELTACAMERA    1.5            /**<  The camera velocity */
#define ZOOMMAXIMO     80             /**< Max value of Zoom */
#define ZOOMMINIMO     280            /**< Min value of Zoom */
#define FARVIEW        SQUARESIZE*500 /**< FarView constant */

#define HALFFARVIEW    FARVIEW / 2.0  /**< Half FarView Value */

/********************************************************************
 *                      Screen's constants                          *
 ********************************************************************/
#define SCREEN_X        800     /**< Screen Width */
#define SCREEN_Y        600     /**< Screen Height */

/********************************************************************
 *                        Sun's constants                           *
 ********************************************************************/
#define SUN_HOUR_BORN   4.5     /**< Sun Hour to born */
#define SUN_HOUR_DEATH 20.0     /**< Sun hour to die */

/*! EQU = (y = bx + c) 
 * x = hour 
 * y = rotation (degrees) */
#define SUN_EQU_B 180.0 / (SUN_HOUR_DEATH - SUN_HOUR_BORN)
#define SUN_EQU_C -(SUN_HOUR_BORN * SUN_EQU_B) /**< same for EQU_B */


/********************************************************************
 *                        Math's constants                          *
 ********************************************************************/
#define TWOPI  2 * M_PI     /**< 2* PI definition */
#define PID2   M_PI / 2.0   /**< PI / 2 definition */
#define PID180 M_PI / 180.0 /**< PI / 180 definition */

/*! degree to radians converter.
 * \param x -> angle in degrees to convert.
 * \return angle in radians. */
inline double deg2Rad(double x){return PID180 * x;}

/*! actualize loading thing on loading briefing screen 
 * \param img -> surface containing the surface to be edited
 * \param texturaTexto -> text texture
 * \param texturaCarga -> Loading image texture 
 * \param texto -> text to be put on loading
 * \param proj -> projection matrix
 * \param modl -> model view matrix
 * \param viewPort -> view port matrix. */
void atualizaCarga(SDL_Surface* img, GLuint* texturaTexto, 
                   GLuint texturaCarga, const char* texto,
                   GLdouble proj[16], GLdouble modl[16],GLint viewPort[4]);

/*! Verifies if some bounding box is inner on another
 * \param min1 -> min values of first bounding box 
 * \param max1 -> max values of first bounding box
 * \param min2 -> min values of second bounding box
 * \param max2 -> max values of second bounding box
 * \param inverso -> OR verifies too if second bounding box is inner to one 
 * \return 1 if bounding 1 is inner to bounding 2. if inverso, return 1 if
 *     bounding 2 is inner to bounding 2 or bounding 2 is inner to bounding 1.*/
int estaDentro(GLfloat min1[3], GLfloat max1[3],
               GLfloat min2[3], GLfloat max2[3],
               int inverso);

/*!
 * Rotate and translate to position the bounding box
 * \param orientacao -> orientation angle
 * \param X -> X bounding box positions
 * \param Z -> Z bounding box positions
 * \param varX -> variation to X position
 * \param varZ -> variation to Z position 
 * \param varMinY -> variation to minY 
 * \param varMaxY -> variation to maxY 
 * \param min -> min points bounding result
 * \param max -> max points bounding result */
void rotTransBoundingBox(GLfloat orientacao, GLfloat X[4], GLfloat Z[4],
                         GLfloat varX, GLfloat varMinY, GLfloat varMaxY, 
                         GLfloat varZ,
                         GLfloat min[3], GLfloat max[3]);

void normal (GLfloat x1, GLfloat y1, GLfloat z1,
             GLfloat x2, GLfloat y2, GLfloat z2,
             GLfloat x3, GLfloat y3, GLfloat z3, 
             GLfloat& nx, GLfloat &ny, GLfloat& nz);

#endif
