#ifndef _collision_h
#define _collision_h

/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <math.h>
#include "../map/map.h"
#include "character.h"

/*! The collision Detection Class */
class collision
{
   public:
      /*! Constructor */
      collision();
      /*! Destructor */
      ~collision();

      /*! Define Current Map
       * \param usedMap -> pointer to current Map
       * \param npcs -> list of NPCs
       * \param pcs -> list of PCs */
      void defineMap(Map* usedMap, characterList* npcs, characterList* pcs);

      /*! Verify if character can occupy position on Square
       * \param min -> min positions of Bounding Box (translated & rotated)
       * \param max -> max positions of bounding box (translated & rotated)
       * \param quad -> pointer to the current square */
      bool verifySquare(GLfloat min[3], GLfloat max[3],Square* quad);

      /*! Verify collision with meios fio
       * \param min -> min positions of Bounding Box (translated & rotated)
       * \param max -> max positions of bounding box (translated & rotated)
       * \param curb -> pointer to the curbs list */
      bool verifyCurb(GLfloat min[3],GLfloat max[3], wall* curb);
      
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
       *  \param varHeight -> value of the new height of the character 
       *  \param nx -> value of the center new X Character
       *  \param nz -> value of the center new Z Character
       *  \return true if the character can occupy the position, 
       *          false otherwise.*/
      /*bool canWalk(GLfloat perX, GLfloat perY, GLfloat perZ, 
                   GLfloat perX1, GLfloat perY1, GLfloat perZ1, 
                   GLfloat perX2, GLfloat perY2, GLfloat perZ2, 
                   GLfloat perOrientation, Square* perQuad,
                   GLfloat& varHeight, GLfloat& nx, GLfloat& nz);*/
      bool canWalk(character* actor, GLfloat varX, GLfloat varY ,GLfloat varZ,
                   GLfloat varAlpha, GLfloat& varHeight, GLfloat& nx, 
                   GLfloat& nz, bool usePosition=true);
                   
      
   private:
      Map* actualMap;         /**< Pointer to current opened map */
      characterList* NPCs;    /**< Pointer to the current NPC List */
      characterList* PCs;     /**< Pointer to the current PC List */
};


#endif

