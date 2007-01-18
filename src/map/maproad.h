#ifndef _maproad_h
#define _maproad_h

/*************************************************************************
 * DccNiTghtmare is Public Domain - Do whatever you want with this code. *
 *************************************************************************/

#include "../engine/util.h"
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_opengl.h>

#define MAPROAD_NONE                0
#define MAPROAD_DOWN_UP             1
#define MAPROAD_CURVE_DOWN_LEFT     2
#define MAPROAD_CURVE_DOWN_RIGHT    3
#define MAPROAD_CURVE_UP_LEFT       4
#define MAPROAD_CURVE_UP_RIGHT      5
#define MAPROAD_LEFT_RIGHT          6
#define MAPROAD_CURVE_LEFT_DOWN     7
#define MAPROAD_CURVE_LEFT_UP       8
#define MAPROAD_CURVE_RIGHT_DOWN    9
#define MAPROAD_CURVE_RIGHT_UP     10
#define MAPROAD_INTERSECTION       11

/*! Definitions of Roads on Map */
class mapRoad
{
   public:
      /*! Constructor
       * \param numX -> map number of squares on X
       * \param numZ -> map number of squares on Z */
      mapRoad(int numX, int numZ);
      /*! Destructor */
      ~mapRoad();

      /*! Set the position to have a road and actualize adjacents roads.
       * \param posX -> X position
       * \param posZ -> Z position 
       * \param direction -> direction of the road */
      void setRoad(int posX, int posZ, int direction);

      /*! Remove road from position and actualize adjacents roads.
       * \param posX -> X position
       * \param posZ -> Z position */
      void unsetRoad(int posX, int posZ);

      /*! Draw all roads to world */
      void draw();

   private:
      int maxX;           /**< Max X Roads */
      int maxZ;           /**< Max Z Roads */
      int** typeOfRoad;   /**< Type of Road Matrix (Directions) */

      GLuint roadTexture;       /**< Road Texture */
      GLuint roadCurveTexture;  /**< Road Curve Texture */
         
};

#endif

