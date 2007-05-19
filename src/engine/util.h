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
 *                           Engine's Constants                     *
 ********************************************************************/

#define ENGINE_MODE_REAL_TIME   1  /**< Real Time Mode, not in battle */
#define ENGINE_MODE_TURN_BATTLE 2  /**< Turn Mode, in battles */

#define ENGINE_WALK_KEYS        1  /**< Engine Keyboard Walk Status */
#define ENGINE_WALK_MOUSE       2  /**< Engine Mouse Walk Status */

//#define ENGINE_ANIMATION_DELAY 4000 /**< Value to delay for end animation */


/********************************************************************
 *                     Actualization constants                      *
 ********************************************************************/
#define REFRESH_RATE 100.0  /**< Keyboard Refresh Rate for the Engine */
#define NORMAL_FPS 35       /**< Minimun FPS to the engine runs smooth */
#define ACTUALIZATION_RATE (1000/NORMAL_FPS)  /**< Actualization Rate in ms */
#define WALK_ACTUALIZATION (1.0/NORMAL_FPS) /**< Walk Actualization */ //0.021

/********************************************************************
 *                Character's moviments constants                   *
 ********************************************************************/

#define ANDAR  WALK_ACTUALIZATION * 37.5 //0.75 /**< The distance that normal character walks */
#define GIRAR  WALK_ACTUALIZATION * 125.0 //2.5 /**< The turn velocity of normal character */
#define WALK_PER_MOVE_ACTION 60 /**< Distance that can be walked per move */

/********************************************************************
 *                      Camera's constants                          *
 ********************************************************************/

#define DELTACAMERA  WALK_ACTUALIZATION * 75 /**<  The camera velocity */ //1.5
#define ZOOMMAXIMO     80             /**< Max value of Zoom */
#define ZOOMMINIMO     500            /**< Min value of Zoom */
#define FARVIEW        4096           /**< FarView constant */

#define HALFFARVIEW    FARVIEW / 2.0  /**< Half FarView Value */

/********************************************************************
 *                      Screen's constants                          *
 ********************************************************************/

#define SCREEN_X        800     /**< Screen Width */
#define SCREEN_Y        600     /**< Screen Height */

/********************************************************************
 *                      Shadow's constants                          *
 ********************************************************************/
#define SHADOW_MAP_SIZE 512     /**< The Shadow Map Size */


/********************************************************************
 *                        Sun's constants                           *
 ********************************************************************/

#define SUN_HOUR_BORN          4.2    /**< Sun Hour to born */
#define SUN_HOUR_DEATH         20.0   /**< Sun hour to die */
#define SUN_HOUR_INITIAL_TOTAL 8.5    /**< Initial hour of Total Light */
#define SUN_HOUR_FINAL_TOTAL  16.5    /**< Final hour of Total Light */

/*! Equations for the sun Position
 * EQU = (y = bx + c) 
 * x = hour 
 * y = rotation (degrees) */

/*! Day Equation */
#define SUN_EQU_B 180.0 / (SUN_HOUR_DEATH - SUN_HOUR_BORN)
#define SUN_EQU_C -(SUN_HOUR_BORN * SUN_EQU_B) /**< same for EQU_B */

/*! Night Equation */
#define SUN_EQN_B 180.0 / (SUN_HOUR_BORN + 24 - SUN_HOUR_DEATH)
#define SUN_EQN_C (180.0 - (SUN_HOUR_DEATH * SUN_EQN_B)) /**< same for EQN_B */



/********************************************************************
 *                        Math's constants                          *
 ********************************************************************/
#ifndef M_PI
   #define M_PI 1.14159     /**< Usually on math.h, but not allways */
#endif

#define TWOPI  2 * M_PI     /**< 2* PI definition */
#define PI     M_PI         /**< PI definition */
#define PID4   M_PI / 4.0   /**< PI / 4 definition */ 
#define PID2   M_PI / 2.0   /**< PI / 2 definition */
#define PID180 M_PI / 180.0 /**< PI / 180 definition */

/********************************************************************
 *                            Functions                             *
 ********************************************************************/


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

/*! Calculate normal to the plane. */
void normal (GLfloat x1, GLfloat y1, GLfloat z1,
             GLfloat x2, GLfloat y2, GLfloat z2,
             GLfloat x3, GLfloat y3, GLfloat z3, 
             GLfloat& nx, GLfloat &ny, GLfloat& nz);

/*! Verify if the target position is in range for the action
 * \param posX -> actor position X
 * \param posZ -> actor position Z
 * \param targX -> target position X
 * \param targZ -> target position Z
 * \param range -> range for the action */
bool actionInRange(GLfloat posX, GLfloat posZ, 
                   GLfloat targX, GLfloat targZ,
                   GLfloat range);

#endif
