#ifndef _collision_h
#define _collision_h

/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <math.h>
#include "../map/map.h"
#include "personagens.h"

/*! The collision Detection Class */
class collision
{
   public:
      /*! Constructor */
      collision();
      /*! Destructor */
      ~collision();

      /*! Define Current Map
       * \param map -> pointer to current Map*/
      void defineMap(Map* map);

      /*! Verify if character can occupy position on Square
       * \param min -> min positions of Bounding Box (translated & rotated)
       * \param max -> max positions of bounding box (translated & rotated)
       * \param quad -> pointer to the current square */
      bool verifySquare(GLfloat min[3], GLfloat max[3],Square* quad);

      /*! Verify collision with meios fio
       * \param min -> min positions of Bounding Box (translated & rotated)
       * \param max -> max positions of bounding box (translated & rotated)
       * \param meiosFio -> pointer to the meios Fio List */
      bool verifyMeioFio(GLfloat min[3],GLfloat max[3], muro* meiosFio);
      
      /*! Verify if character can walk to the position
       *  \param perX -> character X position
       *  \param perY -> character Y position
       *  \param perZ -> character Z position
       *  \param perX1 -> character bounding box X1
       *  \param perY1 -> character bounding box Y1
       *  \param perZ1 -> character bounding box Z1
       *  \param perX2 -> character bounding box X2
       *  \param perY2 -> character bounding box Y2
       *  \param perZ2 -> character bounding box Z2 
       *  \param perOrientation -> character Orientation
       *  \param perQuad -> square occupied by character 
       *  \param NPCs -> list of NPCs
       *  \param varHeight -> value of the new height of the character 
       *  \param nx -> value (in squares) of the new X 
       *  \param nz -> value (in squares) of the new Z
       *  \return true if the character can occupy the position, 
       *          false otherwise.*/
      bool canWalk(GLfloat perX, GLfloat perY, GLfloat perZ, 
                   GLfloat perX1, GLfloat perY1, GLfloat perZ1, 
                   GLfloat perX2, GLfloat perY2, GLfloat perZ2, 
                   GLfloat perOrientation, Square* perQuad,
                   Lpersonagem* NPCs, GLfloat& varHeight,
                   GLfloat& nx, GLfloat& nz);
      
   private:
      Map* actualMap;         /**< Pointer to actual opened map */
};


#endif

