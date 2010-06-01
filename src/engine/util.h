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
#ifndef _dnt_util_h
#define _dnt_util_h

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

#define ENGINE_MODE_REAL_TIME           1  /**< Real Time Mode, not in battle */
#define ENGINE_MODE_TURN_BATTLE         2  /**< Turn Mode, in battles */
#define ENGINE_MODE_WAIT_DIE_ANIMATION  3  /**< Wait Die Animation Mode */

#define ENGINE_WALK_KEYS         1  /**< Engine Keyboard Walk State */
#define ENGINE_WALK_MOUSE        2  /**< Engine Mouse Walk State */
#define ENGINE_WALK_MOUSE_ASTAR  3  /**< Engine A* Mouse Walk State */

#define ENGINE_WALK_ACTION_DELAY 250 /**< Delay before define if is A* or
                                           continuous mouse action */
#define ENGINE_CONTINUOUS_RUN_DISTANCE 60 /**< Distance to toggle walk/run
                                               in continuous mouse state */
#define ENGINE_RUN_MULTIPLIER    2.8f /**< How run is faster than  walk */

#define ENGINE_WAIT_DIE_DELAY  4000 /**< Value to delay for end die animation */

#define ENGINE_WAIT_TURN_DELAY  100 /**< Value to delay between turns */

#define ENGINE_LOADING_IMAGES  6 /**< Total available loading images */

#define DNT_TO_METER   0.5f /**< Constant to multiply to convert meter to DNT.*/
#define METER_TO_DNT   1.0f / DNT_TO_METER /**< Constant to DNT to Meter value*/

#define THUMB_X      128  /**< X size of the screenshot */
#define THUMB_Y       96  /**< Y size of the screenshot */

/********************************************************************
 *                          Update constants                        *
 ********************************************************************/
#define REFRESH_RATE 200.0f  /**< Keyboard Refresh Rate for the Engine */
#define NORMAL_FPS 35       /**< Minimun FPS to the engine runs smooth */
#define UPDATE_RATE (1000 / NORMAL_FPS) /**< Update Rate in ms */
#define WALK_UPDATE (1.0f / NORMAL_FPS) /**< Walk Update Rate */

/********************************************************************
 *                Character's moviments constants                   *
 ********************************************************************/
#define TURN_VALUE (WALK_UPDATE * 125.0f) /**< The turn velocity */
#define WALK_PER_MOVE_ACTION  60 /**< Distance that can be walked per move */
#define DNT_BATTLE_RANGE 3*WALK_PER_MOVE_ACTION /**< Range of init/end battle */

/********************************************************************
 *                      Camera's constants                          *
 ********************************************************************/

#define DELTA_CAMERA  WALK_UPDATE * 75    /**<  The camera velocity */
#define ZOOM_MAX             80            /**< Max value of Zoom */
#define ZOOM_MIN            500            /**< Min value of Zoom */
#define OUTDOOR_FARVIEW    8192            /**< FarView constant */
#define OUTDOOR_FARVIEW_D2 4096            /**< FarView / 2 */
#define OUTDOOR_FARVIEW_D4 2048            /**< Quarter FarView */
#define INDOOR_FARVIEW      768            /**< Indoor FarView */
#define INDOOR_FARVIEW_D2   384            /**< Half Indoor FarView */
#define INDOOR_FARVIEW_D4   192            /**< Quarter Indoor FarView */

/********************************************************************
 *                      Shadow's constants                          *
 ********************************************************************/
#define SHADOW_MAP_SIZE      512     /**< The Shadow Map Size */
#define SHADOWS_NONE           0     /**< No Shadows */
#define SHADOWS_PROJECTIVE     1     /**< Projective Shadows */

#define REFLEXIONS_NONE        0     /**< Don't Do Any Reflexions */
#define REFLEXIONS_CHARACTERS  1     /**< Do Reflexions only on Characters */
#define REFLEXIONS_ALL         2     /**< Do All Reflexions */

/********************************************************************
 *                        Sun's constants                           *
 ********************************************************************/
#define SUN_HOUR_BORN          4.0f    /**< Sun Hour to born */
#define SUN_HOUR_DEATH         20.0f   /**< Sun hour to die */
#define SUN_HOUR_INITIAL_TOTAL 8.5f    /**< Initial hour of Total Light */
#define SUN_HOUR_FINAL_TOTAL  16.5f    /**< Final hour of Total Light */

/*! Equations for the sun Position
 * EQU = (y = bx + c) 
 * x = hour 
 * y = rotation (degrees) */

/*! Day Equation */
#define SUN_EQU_B 180.0f / (SUN_HOUR_DEATH - SUN_HOUR_BORN)
#define SUN_EQU_C -(SUN_HOUR_BORN * SUN_EQU_B) /**< same for EQU_B */

/*! Night Equation */
#define SUN_EQN_B 180.0f / (SUN_HOUR_BORN + 24 - SUN_HOUR_DEATH)
#define SUN_EQN_C (180.0f - (SUN_HOUR_DEATH * SUN_EQN_B)) /**< same for EQN_B */



/********************************************************************
 *                        Math's constants                          *
 ********************************************************************/
#ifndef M_PI
	#define M_PI 3.1415926     /**< Usually on math.h, but not allways */
#endif

#define TWOPI 2 * M_PI      /**< 2* PI definition */
#define PI     M_PI         /**< PI definition */
#define PID4   M_PI / 4.0   /**< PI / 4 definition */ 
#define PID2   M_PI / 2.0   /**< PI / 2 definition */
#define PID180 M_PI / 180.0 /**< PI / 180 definition */
#define PID180I 180.0 / M_PI /**< 180 / PI */

/********************************************************************
 *                            Functions                             *
 ********************************************************************/

/*! degree to radians converter.
 * \param x -> angle in degrees to convert.
 * \return angle in radians. */
inline double deg2Rad(double x){return(PID180 * x);}
/*! radian to degree converter
 * \param y -> angle in radians 
 * \return angle in degree */
inline double rad2Deg(double y){return(y * PID180I);}

/*! actualize loading thing on loading briefing screen 
 * \param img -> surface containing the surface to be edited
 * \param texturaTexto -> text texture
 * \param texturaCarga -> Loading image texture 
 * \param texto -> text to be put on loading
 * \param progress -> progress bar */
void showLoading(SDL_Surface* img, GLuint* texturaTexto, 
                 GLuint texturaCarga, const char* texto,
                 healthBar* progress);

/*! Verifies if some bounding box is inner on another
 * \param min1 -> min values of first bounding box 
 * \param max1 -> max values of first bounding box
 * \param min2 -> min values of second bounding box
 * \param max2 -> max values of second bounding box
 * \return 1 if bounding 1 collides with bounding 2. */
int intercepts(GLfloat min1[3], GLfloat max1[3],
               GLfloat min2[3], GLfloat max2[3]);

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

/*! Set the buffer to 2D draw mode */
void draw2DMode();

/*! Set the buffer to 3D draw mode
 * \param actualFarView -> the actual value of farView to set */
void draw3DMode(float actualFarView);

/*! Read the current front buffer to a SDL_Surface
 * \return -> pointer to the created SDL_Surface with the front buffer image
 * \note -> you must SDL_FreeSurface the result when no more needed! */
SDL_Surface* readFrontBuffer();

/*! Take screenshot from the current buffer
 * \param fileName -> fileName of the screenShot
 * \param thumb -> if will save as a miniature 
 * \return -> true if screenshot was saved */
bool screenshot(string fileName, bool thumb=false); 

/*! Take screenshot from a SDL_Surface
 * \param screen -> pointer to the surface to save, containing ALL the screen
 * \param fileName -> fileName of the screenShot
 * \param thumb -> if will save as a miniature 
 * \return -> true if screenshot was saved */
bool screenshot(SDL_Surface* screen, string fileName, bool thumb);

/*! Calculate the angle with two points
 * \param prvX -> point A X
 * \param prvZ -> point A Z
 * \param curX -> point B X
 * \param curZ -> point B Z
 * \return -> angle in degrees */
GLfloat getAngle(GLfloat prvX, GLfloat prvZ, GLfloat curX, GLfloat curZ);

/*! Normalize a vector
 * \param nx -> vector X 
 * \param ny -> vector Y
 * \param nz -> vector Z */
void normalize (GLfloat& nx, GLfloat& ny, GLfloat& nz);

/*! Normalize a vector
 * \param nx -> vector X 
 * \param ny -> vector Y */
void normalize(GLfloat& nx, GLfloat& ny);

/*! Rotate a point p by angle theta around an arbitrary line segment p1-p2
    Return the rotated point.
    Positive angles are anticlockwise looking down the axis towards the origin.
    Assume right hand coordinate system. */
void rotatePoint(GLfloat srcX, GLfloat srcY, GLfloat srcZ, GLfloat theta,
                 GLfloat p1X, GLfloat p1Y, GLfloat p1Z, 
                 GLfloat p2X, GLfloat p2Y, GLfloat p2Z, 
                 GLfloat& resX, GLfloat& resY, GLfloat& resZ);

/*! Replace all space characteres on the string
 * \param s -> string to replace spaces
 * \return -> string with '_' at spaces places */
string replaceSpaces(string s);

#endif
